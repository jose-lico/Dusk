# Dusk

Dusk is a WIP 3D game engine in C++.<br/>
It is a personal project where I am learning more about how a game engine and all of its systems function under the hood.<br/>
The rendering backend is in OpenGL. It supports both Windows and Linux (tested on Ubuntu).

<p align="center">
<img src="images/dusk_windows.png">
</p>

## Features

Dusk is very early in development and so its features are very barebones.

- Basic rendering
- Phong lighting
- ECS using entt
- Editor using imgui

## Building Dusk

I have only tested building Dusk in VS2019 and G++. There is a focus on making sure Dusk builds on atleast these 2 environments. More might be added in the future.

Dusk Engine is built into a DLL/shared library which is used by the Dusk Editor executable.
All the engines dependencies are built into static libraries which are linked into the engine.

Dusk uses [premake](https://premake.github.io/) as its build system.

1. Clone the repository and its submodules.
2. Run the python script in the scripts folder, or run premake from the root directory and choose the targeted environment.
    - VS2019 `premake5.exe vs2019`
    - GCC &nbsp;&nbsp;&nbsp;&nbsp; `./premake5 gmake2`
3. Compile & run

On Linux you might need to install some packages such as:
  - `libgl1-mesa-dev`
  - `xorg-dev`

## Dependencies

Building and learning about game engines is a massive task, so I am taking some shortcuts where I can and resorting to popular libraries for certain functionality.

  - [GLEW](http://glew.sourceforge.net/) - Cross-platform OpenGL loading library
  - [GLFW](https://www.glfw.org/) - Cross-platform creation of windows, context and input handling
  - [glm](https://github.com/g-truc/glm) - Header only mathematics library for graphics software
  - [stb](https://github.com/nothings/stb) - Header only one-file library to import images (only using [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h))
  - [Dear ImGui](https://github.com/ocornut/imgui) - Immediate mode graphic user interface
  - [spdlog](https://github.com/gabime/spdlog) - Fast C++ logging library
  - [entt](https://github.com/skypjack/entt) - Fast and reliable entity component system (ECS)
