#include <catch2/catch.hpp>

#include <cstdlib>
#include <ctime>
#include <fmt/core.h>

#include "string_cache.hpp"

TEST_CASE("StringCache Simple Load")
{
  core::StringCache cache;

  auto one = cache.GetOrCache("One");
  auto two = cache.GetOrCache("Two");
  auto three = cache.GetOrCache("Three");

  REQUIRE("One" == one->get());
  REQUIRE("Two" == two->get());
  REQUIRE("Three" == three->get());
}

TEST_CASE("StringCache Large Load")
{
  core::StringCache cache;

  std::vector<std::string> randomStrings;
  std::srand((unsigned int) std::time(nullptr)); // use current time as seed for random generator
  auto count = 1000;
  while (count > 0)
  {
    int randomValue = std::rand();
    auto str = fmt::format("{}", randomValue);
    cache.GetOrCache(str);
    randomStrings.push_back(str);
    count--;
  }

  for (const auto& str : randomStrings)
  {
    auto cachedStr = cache.GetOrCache(str);
    REQUIRE(str == cachedStr->get());  
  }
}