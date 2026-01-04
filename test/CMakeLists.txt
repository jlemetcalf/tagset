find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)
find_package(absl REQUIRED)


file(GLOB_RECURSE _SRC_TESTS "*_tests.cpp")
add_executable(tests ${_SRC_TESTS})
target_link_libraries(tests PRIVATE project_warnings project_options fmt::fmt abseil::abseil Catch2::Catch2WithMain tagset_lib)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
include(Catch)
catch_discover_tests(
  tests
  TEST_PREFIX
  "unittests."
  REPORTER
  xml
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests."
  OUTPUT_SUFFIX
  .xml)