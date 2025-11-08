find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shaders
        ${PROJECT_BINARY_DIR}/shaders
        COMMENT "Copying shaders into binary directory")

add_dependencies(${PROJECT_NAME} copy_shaders)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources
        COMMENT "Copying resources into binary directory")

add_dependencies(${PROJECT_NAME} copy_resources)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_clear_color.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_hello_triangle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_texture2D_square.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR} ${PROJECT_SOURCE_DIR}/third-party)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL glad glm::glm imgui stb utils)
