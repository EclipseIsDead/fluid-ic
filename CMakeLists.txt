cmake_minimum_required(VERSION 3.12)
project(fluid)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the source file
set(SOURCES fluid.cpp)

# Find and link against GLFW
find_package(glfw3 REQUIRED)

add_executable(fluid ${SOURCES})

# Set the include directory for GLFW
target_include_directories(fluid PRIVATE /opt/homebrew/Cellar/glfw/3.3.8/include/GLFW)

# Link against GLFW and OpenGL libraries
target_link_libraries(fluid PRIVATE glfw "-framework OpenGL")
