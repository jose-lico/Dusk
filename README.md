# Dusk

**Dusk is a WIP 3D Game Engine**. It is a personal project where I am learning more about how a game engine and all of its systems function under the hood. It is aiming to be a simple tool that can create games for multiple platforms.

<p align="center">
<img src="images/dusk_windows.png">
</p>

## Current Features

Dusk is very early in development and so its features are barebones.

- Uses a build system for the project and all its dependencies
- Logging system
- Simple 3D Rendering
- Ability to import 3D models (.fbx or .obj)
- Phong Lighting
- Material System
- Resource management with UUIDs
- Resource de/serialization (Scenes, Materials) 
- ECS using entt
- Editor tool using imgui

## Goals

It is always good to have goals that push you forward and give you a glimpse of what the future might hold.

- Better editor tool
- Support for multiple graphics APIs
- Concept of a Runtime
- Ability to export games
- Mono Scripting
- Physics
- Audio
- Profiling
- Multithreaded
- PBR workflow
- Concept of Projects

And many many more.

## Building Dusk

*Dusk used to compile on both MSVC for Windows and GCC for Linux. However, at he moment, the GCC setup is broken and the project does not compile sucessfully. This will hopefully be fixed in the future.*

To build Dusk from source, first clone the repository and all of its submodules.

`git clone --recurse-submodules https://github.com/qolisipo/Dusk`<br>

Dusk uses [premake](https://premake.github.io/) as its build system.<br>

To generate the project files, run premake from the project root directory and choose the targeted environment.

- VS2019 `premake5.exe vs2019`
- GCC &nbsp;&nbsp;&nbsp;&nbsp; `./premake5 gmake2`

After these steps the project should compile and run.

## Dependencies

Building and learning about game engines is a massive task, so I am taking some shortcuts where I can and resorting to popular libraries for certain functionality.

  - [GLEW](http://glew.sourceforge.net/) - Cross-platform OpenGL loading library
  - [GLFW](https://www.glfw.org/) - Cross-platform creation of windows, context and input handling
  - [glm](https://github.com/g-truc/glm) - Header only mathematics library for graphics software
  - [stb](https://github.com/nothings/stb) - Header only one-file library to import images (only using [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h))
  - [Dear ImGui](https://github.com/ocornut/imgui) - Immediate mode graphic user interface
  - [spdlog](https://github.com/gabime/spdlog) - Fast C++ logging library
  - [entt](https://github.com/skypjack/entt) - Fast and reliable entity component system (ECS)
