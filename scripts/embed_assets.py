import os
import io
import struct
import hashlib
from PIL import Image

def bytes_to_c_arr(data, lowercase=True):
    return [format(b, '#04x' if lowercase else '#04X') for b in data]

def generate_embedded(path, file_type, extension, data=None, md5=None):
    
    # open data if it did not exist
    if(data is None):
        data = open(path, "rb").read() if file_type == "binary" else open(path, "r").read()

    # create md5 file
    if(md5 is None):
        md5 = hashlib.md5(data).hexdigest()

    md5_file = open(path + ".md5", "w")
    md5_file.write(md5)
    md5_file.close() 
    
    # embed asset according to extension
    if(extension == ".png" or extension == ".jpg" or extension == ".jpeg"):
        image = Image.open(io.BytesIO(data))
        image = image.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
        image_embedded = open(path + ".embedded", "wb")
        image_embedded.write(struct.pack("@IIQQ", image.width, image.height, len(image.getbands()), len(image.tobytes())))
        image_embedded.write(image.tobytes())
        image_embedded.close()
    elif(extension == ".ttf"):
        c_array_name = "EMBEDDED_" + filename_tup[0].upper().replace("-", "_") # replace potential dangerous chars

        c_array = "static const unsigned char {0}[] = {{{1}}};".format(c_array_name, ", ".join(bytes_to_c_arr(data)))
        c_array_file = open(path + ".embedded", "w")
        c_array_file.write(c_array)
        c_array_file.close()

embedded_extensions = [".ttf", ".png", "jpg", ".jpeg"] # only embed files with these extensions

cwd = os.getcwd()
embedded_dir = cwd + "\embedded"

# iterate files inside embbedded folder
for root, subdirs, files in os.walk(embedded_dir):
    for filename in files:
        filename_tup =  os.path.splitext(filename)
        filepath = os.path.join(root, filename)
        filepath_tup = os.path.splitext(filepath)

        extension = None

        for ext in embedded_extensions:
            if(filepath_tup[1] == ext):
                extension = ext
                break
        
        # if the the file does not have one of the specified extensions, continue
        if not (extension):
            continue
        
        file_type = None

        # get file type
        try:
            with open(filepath, "r") as f:
                data = f.read(1024)
                file_type = "text"
        except UnicodeDecodeError:
            file_type = "binary"

        if(os.path.exists(filepath + ".md5")): # if the asset has an md5
            if(os.path.exists(filepath + ".embedded")): #if the asset has an embedded version
                old_md5 = open(filepath + ".md5", "r").read()
                data = open(filepath, "rb").read() if file_type == "binary" else open(filepath, "r").read()
                new_md5 = hashlib.md5(data).hexdigest()
                if not (old_md5 == new_md5): # if the md5 of the embedded version original file does not match the new, regenerate
                    print(filename + " has been updated. Regenerating md5 and embedded asset.")
                    generate_embedded(filepath, file_type, extension, data, new_md5)
            else: # if the embedded file is not present (Probably delete manually)
                print("Can not find embedded asset for " + filename + ". Regenerating md5 and embedded asset.")
                generate_embedded(filepath, file_type, extension)
        else: # if the asset does not have an md5, create md5 and import
            print("Generating md5 and embedded asset for " + filename)
            generate_embedded(filepath, file_type, extension)