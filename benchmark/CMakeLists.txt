find_package(benchmark REQUIRED)
find_package(fmt REQUIRED)
find_package(absl REQUIRED)

file(GLOB_RECURSE _SRC "*.cpp")
add_executable(benchmark ${_SRC})
target_link_libraries(benchmark PRIVATE project_options project_warnings fmt::fmt benchmark::benchmark_main abseil::abseil tagset_lib)
