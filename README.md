# Point Light Shadow Demo

This project implements a simple OpenGL renderer which is capable of simulating
point light shadows. It features

- point light shadows
- a simple scene graph
- basic input handling and camera controls
- the capability to load obj files



## Requirements

- C++17 compiler
- CMake
- A GPU supporting OpenGL 4.0



## Building the project
This project uses CMake for its build system. The project was tested on Windows
using Visual Studio 2019 and an Nvidia GPU. It should work on Linux as well, but
it may not work out of the box.



### MSVC on Windows
On Windows use the CMake GUI to build this project. First create a new directory
called "build" next to this README file. In the CMake GUI set "Where to build
the binaries" to this newly created build directory. Set "Where is the source 
code" to the directory which contains the "CMakeLists.txt" file. This file is 
located next to this README. Next click "Configure". Then click "Generate". The
solution file will be created within the build directory.



### Linux
On Linux it should be sufficient to navigate to the directory of this README 
file and execute the following commands:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```