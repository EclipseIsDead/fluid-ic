# Fluid-ic

A minimal fluid simulator in C++ (mov -> gif converter killed the fps).

![Demo](https://github.com/EclipseIsDead/fluid-ic/blob/main/demo.gif)

## Dependencies
- OpenGL 3.3
- GLFW3
- GLAD

## Build

Change the main shader paths in fluid.cpp and use absolute pathing. I will fix this later. Included fluid in build for an initial executable. To build yourself, run:

```bash
cd build
cmake ..
make
./fluid
```

Note:
- Read CMakeLists.txt for more details on build.
- Assumes brew install of GLFW3 and GLM.

If you want, you can delete dependencies and move the include and lib contents to usr/local/include and usr/local/lib respectively. Just adjust the CMakeLists.txt file accordingly.

## Todo
- [ ] Hardcoded shader path, should be dynamic on both WIN32 and MacOS.
- [ ] Better dynamic system?
