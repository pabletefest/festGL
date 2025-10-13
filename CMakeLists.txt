cmake_minimum_required(VERSION 3.31)
project(OpenGLRenderer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
# set(CMAKE_CSTANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMADNS ON)

# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#
# Dependencies
#

include(FetchContent)

# GLFW
find_package(glfw3 3.4 QUIET)
if (NOT glfw3_FOUND)
    FetchContent_Declare(
            glfw3
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    )
    FetchContent_GetProperties(glfw3)
    if (NOT glfw3_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(glfw3)
        # add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
    endif()
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
find_package(glad 2.0.8 QUIET)
if (NOT glad_FOUND)
    FetchContent_Declare(
            glad
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/Dav1dde/glad/archive/refs/tags/v2.0.8.zip
            SOURCE_SUBDIR  cmake
    )
    FetchContent_GetProperties(glad)
    if(NOT glad_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(glad)

        # add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
        glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6)
    endif()
endif()
set_target_properties(glad PROPERTIES FOLDER "Dependencies")

# GLM
find_package(glm 1.0.1 QUIET)
if (NOT glm_FOUND)
    FetchContent_Declare(
            glm
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    )
    FetchContent_GetProperties(glm)
    if (NOT glm_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(glm)
        # add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
    endif()
endif()
set_target_properties(glm PROPERTIES FOLDER "Dependencies")

FetchContent_Declare(imgui_external
	URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.2b.tar.gz
	EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(imgui_external)

add_library(imgui
	${imgui_external_SOURCE_DIR}/imgui.cpp
	${imgui_external_SOURCE_DIR}/imgui_draw.cpp
	${imgui_external_SOURCE_DIR}/imgui_tables.cpp
	${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_external_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${imgui_external_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR})

target_link_libraries(imgui PRIVATE glfw)

# add_subdirectory(third-party/GLFW)
# add_subdirectory(third-party/Glad)
# add_subdirectory(third-party/GLM)
# add_subdirectory(third-party/ImGui)

#
#   Project
#

add_subdirectory(src)