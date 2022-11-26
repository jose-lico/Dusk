import os
import hashlib

cwd = os.getcwd()
embedded_dir = cwd + "\embedded"

def bytes_to_c_arr(data, lowercase=True):
    return [format(b, '#04x' if lowercase else '#04X') for b in data]

# iterate files inside embbedded folder
for root, subdirs, files in os.walk(embedded_dir):
    for filename in files:
        filename_tup =  os.path.splitext(filename)
        filepath = os.path.join(root, filename)
        filepath_tup = os.path.splitext(filepath)
        
        # only doing .ttf for now to test
        if(filepath_tup[1] != ".ttf"):
            continue

        # if the file is an md5 or the embedded target, continue
        if(filepath_tup[1] == ".md5" or filepath_tup[1] == ".embedded"):
            continue
        
        if(os.path.exists(filepath + ".md5")): # if the asset has an md5
            if(os.path.exists(filepath + "embedded")): #if the asset has an embedded version
                print("Comparing md5") # if md5 is different, reimport and write md5, else do noting
            else: # if the asset does not have an embedded version (deleted manually probably), import and create md5
                print("TODO")
        else: # if the asset does not have an md5, create md5 and import
            print(filename + " does not have md5")
            data = open(filepath, "rb").read()
            
            md5 = hashlib.md5(data).hexdigest()
            md5_file = open(filepath + ".md5", "w")
            md5_file.write(md5)
            md5_file.close()

            c_array = "static const unsigned char {0}[] = {{{1}}};".format("EMBEDDED_" + filename_tup[0].upper().replace("-", "_"), ", ".join(bytes_to_c_arr(data)))
            c_array_file = open(filepath + ".embedded", "w")
            c_array_file.write(c_array)
            c_array_file.close()        