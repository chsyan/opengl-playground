Just a repo where I play around with and learn opengl.
Only works on a Linux env.

# Dependencies
- [CMake](https://gitlab.kitware.com/cmake/cmake)
- [OpenGL](https://www.opengl.org/)
- [Jinja2](https://pypi.org/project/Jinja2/)

# Installation
### Getting the source
The source code lives on GitHub:
```sh
$ git clone --recurse-submodules https://github.com/chsyan/opengl-stuff
$ cd opengl-stuff
$ git submodule update --remote
```
You can also get the code [as a Zip file](https://github.com/chsyan/opengl-stuff/archive/master.zip).

### Building
This project is built using [CMake](https://gitlab.kitware.com/cmake/cmake). 
In source build is not recommended, so build the project inside a build directory.
```sh
$ cd path/to/opengl-stuff
$ mkdir build
$ cd build
$ cmake ..
$ make all
$ cd bin
$ ./opengl-stuff # Run the application
```
