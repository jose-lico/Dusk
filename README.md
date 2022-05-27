# Dusk

**Dusk is a WIP 3D Game Engine**. It is a personal project where I am learning more about how a game engine and all of its systems function under the hood. It is aiming to be a simple tool that can create games for multiple platforms.

<p align="center">
<img src=".github/images/dusk_windows.png">
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
  * [Screenshots](#screenshots)
  

# The Story

If you are interested in reading about the journey that led me to this project, refer [here](https://github.com/qolisipo/Dusk/blob/main/.github/STORY.md).

# Features

Dusk is very early in development and so its features are barebones.

- Build system for the project and all its dependencies
- Logging system
- 3D Rendering
- Ability to import 3D models (.fbx or .obj)
- Material System
- Asset management with UUIDs
- ECS
- Lua Scripting
- Fully fledged editor
- Simple Runtime
  
# Goals

It is always good to have goals that push you forward and give you a glimpse of what the future might hold.

Dusk's short term goal is to reach the MVP stage of a game engine. For me, this means to be able to create and manipulate scenes and the objects
that live in it, create assets such as materials, and finally having the concept of a runtime and being able to export a game build which can be distribuited.

Here are some of the tasks towards this goal, and beyond.

- Better editor tool
- Packaging & export of assets
- Profiling
- Multithreaded
- PBR workflow
- Support for multiple graphics APIs
- Physics
- Audio

And many many more.

# Getting Dusk

## Building Dusk

*I only test Dusk on Linux every so often so it is possible that the project does not compile with gcc from time to time.*

To build Dusk from source, first clone the repository and all of its submodules.

`git clone --recurse-submodules https://github.com/qolisipo/Dusk`<br>

Dusk uses [premake](https://premake.github.io/) as its build system.<br>

To generate the project files, run premake from the project root directory and choose the targeted environment.

- VS2019 `premake5.exe vs2022`
- GCC &nbsp;&nbsp;&nbsp;&nbsp; `./premake5 gmake2`

After these steps the project should compile and run.

## Binaries

For the time being, Dusk is only distributed as source code. Once the MVP stage is reached, a build for both Windows and Linux will be available on the [Releases tab](https://github.com/qolisipo/Dusk/releases). 

# Dependencies

Building a game engine is a massive task, and although I am trying to learn as much as possible about every nook and crany, sometimes shortcuts must be taken so I can maximize my time, effort and focus. Therefore I am resorting to popular libraries for certain functionality.
Some of these libraries will be swapped out in the future for a custom solution, once I feel like it is time/the engine is more mature.

[Dependencies List](https://github.com/qolisipo/Dusk/blob/main/.github/DEPENDENCIES.md)

# Resources

Building this project has been quite the journey. I could never have made it this far without all the resources out there pointing me in the right direction and providing motivation along the way.<br>

In order to remain as wary and absorb as much information as possible, I try to look at lots of different sources, always cross checking information and making sure to follow it up with the comments on the video or article. I find this last aspect very important, as it can  lead me to new directions and prevent common gotchas.<br>

Interacting, discussing and learning from other devs has also been part of my learning experience, and has helped me push my knowledge and this project even further.

Among all the resources I used, there is one that stands out.<br> 
[The Cherno](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw) is a youtuber who has been developing his own engine called Hazel, recording tutorials and dev-logs of the process. He also has a lot of other videos on topics such as C++, game engines, rendering, and more.<br> 
Dusk's early development was very inspired by Hazel as I was still riding this bicycle with training wheels, but has since grown a lot, both in terms of implementation and goals.

[Resources List](https://github.com/qolisipo/Dusk/blob/main/.github/RESOURCES.md)

# Screenshots

<p align="center">
<img src=".github/images/dusk_linux.png">
</p>

*Dusk running on Linux*