cmake_minimum_required(VERSION 3.12)
project(fluid-ic)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# local include directory for GLAD
set(CMAKE_REQUIRED_INCLUDES "/usr/local/include")
include_directories("/usr/local/include")

# Add the source files
set(SOURCES main/fluid.cpp main/physics.cpp)

# Find and link against GLFW and GLAD
find_package(glfw3 REQUIRED)

add_subdirectory(dependencies)
add_executable(fluid ${SOURCES})

target_link_libraries(fluid
    PRIVATE
        glfw
        glad
        "-framework OpenGL"  # Add this line to link against the OpenGL library
)

target_include_directories(fluid
    PRIVATE
        "main/"
)

# Set the include directory for GLFW, assuming brew
target_include_directories(fluid PRIVATE "/opt/homebrew/include")
