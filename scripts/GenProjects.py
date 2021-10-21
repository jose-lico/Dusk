import os
import platform as pl

platform = pl.system()

os.chdir("..")
if(platform == "Windows"):
    os.system("premake5.exe vs2019")
    os.system("PAUSE")
elif(platform == "Linux"):
    os.system("./premake5 gmake2")
