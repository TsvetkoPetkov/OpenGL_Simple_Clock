# Clock Simulation (OpenGL, CMake, Windows)

Minimal OpenGL clock using GLFW (no GLAD). Renders a circular clock face and animates hour, minute, and second hands based on local time.

## Prerequisites

- Windows 10/11
- Visual Studio 2019/2022 (Desktop development with C++) or VS Build Tools
- CMake 3.20+
- Git

No vcpkg. CMake will fetch and build GLFW automatically during configure. The script does not install CMake or Visual Studio.

## Build

- Build (installs libraries if needed, configures and builds):

  - `scripts\build.bat` or `scripts\build.bat Release`

- Clean build artifacts:

  - `scripts\clean.bat`

Artifacts are placed in `build/`.

## Notes

- CMake target: `clock_sim`
- Source entry point: `src\main.cpp`
- CMake FetchContent downloads/builds GLFW at configure time.
- On first run, the window defaults to 800x800 with vsync enabled.
