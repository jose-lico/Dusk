import os
import io
import struct
import hashlib
from PIL import Image

def bytes_to_c_arr(data, lowercase=True):
    return [format(b, '#04x' if lowercase else '#04X') for b in data]

def remove_comment_from_shader(shader_line):
    comment = shader_line.find('//')
    if comment == -1:
        return shader_line
    return shader_line[:comment] 

def generate_embedded(path, file_type, extension, data=None, md5=None):
    
    # open data if it did not exist
    if(data is None):
        data = open(path, "rb").read() if file_type == "binary" else open(path, "r").read()

    # create md5 file
    if(md5 is None):
        md5 = hashlib.md5(data).hexdigest() if file_type == "binary" else hashlib.md5(bytes(data, encoding='utf-8')).hexdigest()

    md5_file = open(path + ".md5", "w")
    md5_file.write(md5)
    md5_file.close() 
    
    # embed asset according to extension
    if(extension == ".glsl"):
        shader_source = ["", ""]
        shader_type = None

        for line in open(path, "r").readlines():
            if (not line.find("#shader") == -1):
                if (not line.find("vertex") == -1):
                    shader_type = 0
                if (not line.find("fragment") == -1):
                    shader_type = 1
            else:
                shader_source[shader_type] += '"' + remove_comment_from_shader(line.rstrip()) + ('"\n' if(not shader_source[shader_type] == "") else '""\\n"') 

        for i in range(len(shader_source)):
            c_array_name = "EMBEDDED_" + filename_tup[0].upper().replace("-", "_") + ("VERT" if (i == 0) else "FRAG") 
            c_array = "static const unsigned char {0}[] = {{{1}}};".format(c_array_name, shader_source[i])
            c_array_file = open(path + (".vert.embedded" if (i == 0) else ".frag.embedded"), "w")
            c_array_file.write(c_array)
            c_array_file.close()

        return

    if(extension == ".png" or extension == ".jpg" or extension == ".jpeg"):
        image = Image.open(io.BytesIO(data))
        image = image.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
        # optimally would just write to a variable but fuck it
        image_embedded = open(path + ".embedded", "wb")
        image_embedded.write(struct.pack("@IIQQ", image.width, image.height, len(image.getbands()), len(image.tobytes())))
        image_embedded.write(image.tobytes())
        image_embedded.close()
        data = open(path + ".embedded", "rb").read()
        os.remove(path + ".embedded")

    c_array_name = "EMBEDDED_" + filename_tup[0].upper().replace("-", "_") # replace illegal chars
    c_array = "static const unsigned char {0}[] = {{{1}}};".format(c_array_name, ", ".join(bytes_to_c_arr(data)))
    c_array_file = open(path + ".embedded", "w")
    c_array_file.write(c_array)
    c_array_file.close()

embedded_extensions = [".ttf", ".png", "jpg", ".jpeg", ".glsl", ".ini"] # only embed files with these extensions

cwd = os.getcwd()
embedded_dir = cwd + "\embedded"

# iterate files inside embbedded folder
for root, subdirs, files in os.walk(embedded_dir):
    for filename in files:
        filename_tup =  os.path.splitext(filename)
        filepath = os.path.join(root, filename)
        filepath_tup = os.path.splitext(filepath)

        extension = None

        # if the the file does not have one of the specified extensions, continue
        for ext in embedded_extensions:
            if(filepath_tup[1] == ext):
                extension = ext
                break
        
        if not (extension):
            continue
        
        file_type = None

        # get file type
        try:
            with open(filepath, "r") as f:
                data = f.read(512)
                file_type = "text"
        except UnicodeDecodeError: 
            file_type = "binary"

        if(os.path.exists(filepath + ".md5")): # if the asset has an md5
            if(os.path.exists(filepath + ".embedded") and not extension == ".glsl"): #if the asset has an embedded version. in the case of shaders there are 2 embedded file so check for that
                old_md5 = open(filepath + ".md5", "r").read()
                data = open(filepath, "rb").read() if file_type == "binary" else open(filepath, "r").read()
                new_md5 = hashlib.md5(data).hexdigest()
                if not (old_md5 == new_md5): # if the md5 of the embedded version original file does not match the new, regenerate
                    print(filename + " has been updated. Regenerating md5 and embedded asset.")
                    generate_embedded(filepath, file_type, extension, data, new_md5)
            elif(extension == ".glsl"): # if the asset is a shadr, check that both vert and frag files are present
                if(os.path.exists(filepath + ".vert.embedded") and os.path.exists(filepath + ".frag.embedded")):
                    old_md5 = open(filepath + ".md5", "r").read()
                    data = open(filepath, "r").read()
                    new_md5 = hashlib.md5(bytes(data, encoding='utf-8')).hexdigest()
                    if not (old_md5 == new_md5): # if the md5 of the embedded version original file does not match the new, regenerate
                        print(filename + " has been updated. Regenerating md5 and embedded assets.")
                        generate_embedded(filepath, file_type, extension, data, new_md5)
                else:
                    print("Can not find embedded assets for " + filename + ". Regenerating md5 and embedded assets.")
                    generate_embedded(filepath, file_type, extension)

            else: # if the embedded file is not present (Probably delete manually)
                print("Can not find embedded asset for " + filename + ". Regenerating md5 and embedded asset.")
                generate_embedded(filepath, file_type, extension)
        else: # if the asset does not have an md5, create md5 and import
            print("Generating md5 and embedded asset for " + filename)
            generate_embedded(filepath, file_type, extension)