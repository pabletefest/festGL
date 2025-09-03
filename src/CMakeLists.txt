find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_clear_color.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL Glad glm::glm imgui)