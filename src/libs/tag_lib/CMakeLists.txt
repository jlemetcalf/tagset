set(TARGET tag_lib)
file(GLOB_RECURSE _HDRS "*.hpp")
file(GLOB_RECURSE _SRCS "*.[hc]pp")

add_library (${TARGET} ${_HDRS} ${_SRCS})
target_include_directories (${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ${TARGET}
  PRIVATE project_options
          CONAN_PKG::fmt
          CONAN_PKG::spdlog)