cmake_minimum_required(VERSION 3.31)
project(OpenGLRenderer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
# set(CMAKE_CSTANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMADNS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(src)
add_subdirectory(third-party/GLFW)
add_subdirectory(third-party/Glad)
add_subdirectory(third-party/GLM)
