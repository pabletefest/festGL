add_library(utils STATIC)

target_sources(utils
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/image_loader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/file_reader.cpp

  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/image_loader.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/file_reader.hpp
)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(utils PRIVATE stb)