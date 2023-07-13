# Fluid-ic

A minimal fluid simulator in C++.

## Dependencies
- OpenGL 3.3
- GLFW3
- GLAD

## Build

Included fluid in build for an initial executable. To build yourself, run:

```bash
cd build
cmake ..
make
./fluid
```

Note:
- Read CMakeLists.txt for more details on build.
- Assumes brew install of GLFW3

If you want, you can delete dependencies and move the include and lib contents to usr/local/include and usr/local/lib respectively. Just adjust the CMakeLists.txt file accordingly.