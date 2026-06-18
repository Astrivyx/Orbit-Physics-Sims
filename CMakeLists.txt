cmake_minimum_required(VERSION 3.15)

set(VCPKG_MANIFEST_MODE ON) # Forces CMake to read your vcpkg.json
set(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)

project(OpenGLProject CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Define your executable
add_executable(opengl_app main.cpp)

# Link the libraries to your executable
target_link_libraries(opengl_app PRIVATE 
    OpenGL::GL
    glfw
    glad::glad 
    glm::glm
)