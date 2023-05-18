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

def generate_embedded(path, file_type, extension, data, md5=None):
    # create md5 file if it does not exist
    if(md5 is None):
        md5 = hashlib.md5(data).hexdigest() if file_type == "binary" else hashlib.md5(bytes(data, encoding='utf-8')).hexdigest()

    md5_file = open(path + ".md5", "w")
    md5_file.write(md5)
    md5_file.close() 
    
    array_name = os.path.splitext(os.path.basename(path))[0]

    # embed asset according to extension

    # if shader (only text format being embedded)
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
            c_array_name = "EMBEDDED_" + array_name.upper().replace("-", "_") + ("VERT" if (i == 0) else "FRAG") 
            c_array = "static const unsigned char {0}[] = {{{1}}};".format(c_array_name, shader_source[i])
            c_array_file = open(path + (".vert.embedded" if (i == 0) else ".frag.embedded"), "w")
            c_array_file.write(c_array)
            c_array_file.close()
    else: #if binary
        if(extension == ".png" or extension == ".jpg" or extension == ".jpeg"): #if image, decode image and add engine expected header
            image = Image.open(io.BytesIO(data))
            image = image.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
            buffer = io.BytesIO()
            buffer.write(struct.pack("@IIQQQ", image.width, image.height, len(image.getbands()), len(image.tobytes()), 0))
            buffer.write(image.tobytes())
            buffer.seek(0)
            data = buffer.read()

        # write binary to file
        c_array_name = "EMBEDDED_" + array_name.upper().replace("-", "_") # replace illegal chars
        c_array = "static const unsigned char {0}[] = {{{1}}};".format(c_array_name, ", ".join(bytes_to_c_arr(data)))
        c_array_file = open(path + ".embedded", "w")
        c_array_file.write(c_array)
        c_array_file.close()

def main():
    cwd = os.getcwd()
    embedded_dir = os.path.join(cwd, "embedded")
    
    # iterate files inside embbedded folder
    for root, subdirs, files in os.walk(embedded_dir):
        for filename in files:
            _ = subdirs

            filepath = os.path.join(root, filename)
            filepath_tup = os.path.splitext(filepath)

            # if the the file does not have one of the specified extensions, continue
            extension = filepath_tup[1]
            not_found = True

            embedded_extensions = [".ttf", ".png", ".jpg", ".jpeg", ".glsl", ".ini"]

            for ext in embedded_extensions:
                if extension == ext:
                    not_found = False
                    break
            
            if not_found:
                continue
            
            # get file type
            file_type = None

            try:
                with open(filepath, "r") as f:
                    data = f.read(512)
                    file_type = "text"
            except UnicodeDecodeError: 
                file_type = "binary"

            data = open(filepath, "rb").read() if file_type == "binary" else open(filepath, "r").read()

            if os.path.exists(filepath + ".md5"): # if the asset has an md5
                if os.path.exists(filepath + ".embedded") or (os.path.exists(filepath + ".vert.embedded") and os.path.exists(filepath + ".frag.embedded")): #if the asset has an embedded version
                    old_md5 = open(filepath + ".md5", "r").read()
                    new_md5 = hashlib.md5(data if file_type == "binary" else bytes(data, encoding='utf-8')).hexdigest()
                    if not (old_md5 == new_md5): # if the md5 of the embedded version does not match the file, regenerate, else, everything is gucci
                        print(filename + " has been updated. Regenerating md5 and embedded asset.")
                        generate_embedded(filepath, file_type, extension, data, new_md5)
                else: # if the embedded file is not present (Probably deleted manually)
                    print("Can not find embedded asset for " + filename + ". Regenerating md5 and embedded asset.")
                    generate_embedded(filepath, file_type, extension, data)
            else: # if the asset does not have an md5, create md5 and create embedded
                print("Generating md5 and embedded asset for " + filename)
                generate_embedded(filepath, file_type, extension, data)

if __name__ == "__main__":
    main()
