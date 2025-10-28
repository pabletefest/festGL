add_library(stb STATIC)

target_sources(stb
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp

  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h
)

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})