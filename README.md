# Raylib-ImGui template
A simple cross platform C++ template for using ImGui with Raylib.
It integrate [ImGui](https://github.com/ocornut/imgui/tree/docking) (the docking branch), using the [rlImGui](https://github.com/raylib-extras/rlImGui) backend, and [ImPlot](https://github.com/epezent/implot) With [Raylib](https://github.com/raysan5/raylib) for rapid prototyping.

It uses [premake5](https://premake.github.io/) for the build system.
## Supported Platforms
* Windows 
* Linux

# Build
- Make sure you have the needed software to use [raylib](https://github.com/raysan5/raylib)
- For linus you may need to install the needed dependancies
  - On Ubuntu : 
``` bash
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```
- Please consult the raylib [Wiki](https://github.com/raysan5/raylib/wiki) for more details on your specefic platform
## Windows : Visual Studio 2022
- generate the solution using `generate_solution.bat` which will generate the visula studio 2022 solution files (.sln) using premake
## Linux : GNU Make
* cd into the `build` folder
* run `./premake5 gmake2` to generate the `MakeFile` (in the root directory)
* cd back to the root
* run `make`

# Note
- This template has been greatly inspired by [raylib-quickstart](https://github.com/raylib-extras/raylib-quickstart)