# Dusk

**Dusk is a WIP 3D Game Engine**. It is a personal project where I am learning more about how a game engine and all of its systems function under the hood. It is aiming to be a simple tool that can create games for multiple platforms.

<p align="center">
<img src="images/dusk_windows.png">
</p>

# Table of Contents

  * [The Story](#the-story)
  * [Features](#features)
  * [Goals](#goals)
  * [Getting Dusk](#getting-dusk)
    * [Building Dusk](#building-dusk)
    * [Binaries](#binaries)
  * [Dependencies](#dependencies)
  * [Resources](#resources)
  

# The Story

If you are interested in reading about the journey that led me to this project, refer [here](link-to-story).

# Features

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

# Goals

It is always good to have goals that push you forward and give you a glimpse of what the future might hold.

Dusk's short term goal is to reach the MVP stage of a game engine. For me, this means to be able to create and manipulate scenes and the objects
that live in it, create assets such as materials, and finally having the concept of a runtime and being able to export a game build which can be distribuited.
This MVP will not even have scripting meaning the exported build will most likely just be a static scene.

Here are some of the tasks towards this goal, and beyond.

- Better editor tool
- Concept of a Runtime
- Ability to export games
- Profiling
- Multithreaded
- PBR workflow
- Support for multiple graphics APIs
- Mono Scripting
- Physics
- Audio

And many many more.

# Getting Dusk

## Building Dusk

*Dusk used to compile on both MSVC for Windows and GCC for Linux. However, at the moment, the GCC setup is broken and the project does not compile sucessfully. This will hopefully be fixed in the future.*

To build Dusk from source, first clone the repository and all of its submodules.

`git clone --recurse-submodules https://github.com/qolisipo/Dusk`<br>

Dusk uses [premake](https://premake.github.io/) as its build system.<br>

To generate the project files, run premake from the project root directory and choose the targeted environment.

- VS2019 `premake5.exe vs2019`
- GCC &nbsp;&nbsp;&nbsp;&nbsp; `./premake5 gmake2`

After these steps the project should compile and run.

## Binaries

For the time being, Dusk is only distributed as source code. Once the MVP stage is reached, a build for both Windows and Linux will be available on the [Releases tab](https://github.com/qolisipo/Dusk/releases). 

# Dependencies

Building a game engine is a massive task, and although I am trying to learn as much as possible about every nook and crany, sometimes shortcuts must be taken so I can maximize my time, effort and focus. Therefore I am resorting to popular libraries for certain functionality.
Some of these libraries will be swapped out in the future for a custom solution, once I feel like it is time/the engine is more mature.

[Dependencies List](https://github.com/qolisipo/Dusk/blob/main/DEPENDENCIES.md)

# Resources
