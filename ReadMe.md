# Fluid-ic

A minimal fluid simulator in C++.

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
- Assumes brew install of glfw3 (include path and library path are hardcoded in CMakeLists.txt)