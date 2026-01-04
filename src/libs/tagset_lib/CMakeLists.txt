set(TARGET tagset_lib)
file(GLOB_RECURSE _HDRS "*.hpp")
file(GLOB_RECURSE _SRCS "*.[hc]pp")

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(absl REQUIRED)

add_library (${TARGET} ${_HDRS} ${_SRCS})
target_include_directories (${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ${TARGET}
  PRIVATE project_options
          fmt::fmt
          spdlog::spdlog
          abseil::abseil)