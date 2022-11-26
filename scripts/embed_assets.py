import os
import hashlib

cwd = os.getcwd()
embedded_dir = cwd + "\embedded"

for root, subdirs, files in os.walk(embedded_dir):
    for filename in files:
        file_path = os.path.join(root, filename)
        split_tup = os.path.splitext(file_path)
        if(split_tup[1] == ".txt"):
            f = open(file_path, 'r')
            print(f.read())
        