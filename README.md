# Dusk

Dusk is a 3D game engine written in C++. It is a personal project with the focus of learning more about how a game engine is made and how it functions under the hood.
The rendering backend is in OpenGL. It supports both Windows and Linux (tested on Ubuntu).

## Building Dusk

I have only tested building Dusk in VS2019 and GCC. There is a focus on making sure Dusk runs on atleast these 2 environments. More might be added in the future.

Dusk Engine is built into a DLL/shared library which is used by the Dusk Editor executable.
All the engines dependencies are built into static libraries which are linked into the engine.

Dusk uses [premake](https://premake.github.io/) as its build system.

1. Clone the repository and its submodules.
2. Run premake from the root directory and choose the targeted environment.
    - VS2019 `premake5.exe vs2019`   
    - GCC &nbsp;&nbsp;&nbsp;&nbsp; `./premake5 gmake2`
3. Compile & run

On Linux you might need to install some packages such as:
  - `libgl1-mesa-dev`
  - `xorg-dev`

## Dependencies

Building and learning about game engines is a massive task, so I am taking some shortcuts where I can and resorting to popular libraries for certain functionality.

  - [GLEW](http://glew.sourceforge.net/) Cross-platform OpenGL loading library
  - [GLFW](https://www.glfw.org/) Cross-platform creation of windows, context and input handling
  - [glm](https://github.com/g-truc/glm) Header only mathematics library for graphics software
  - [stb](https://github.com/nothings/stb) Header only one-file library to import images (only using [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h))
  - [Dear ImGui](https://github.com/ocornut/imgui) Immediate mode graphic user interface
