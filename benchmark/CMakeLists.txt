#add_library(benchmark_main STATIC main.cpp)
#target_link_libraries(benchmark_main PUBLIC CONAN_PKG::benchmark)

file(GLOB_RECURSE _SRC "*.cpp")
add_executable(benchmark ${_SRC})
target_link_libraries(benchmark PRIVATE project_options project_warnings CONAN_PKG::fmt CONAN_PKG::benchmark CONAN_PKG::abseil tagset_lib)
