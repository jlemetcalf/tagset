#include <catch2/catch_all.hpp>

#include <cstdlib>
#include <ctime>
#include <fmt/core.h>

#include "tag_factory.hpp"

TEST_CASE("Tag Simple Test")
{
  tagset::TagFactory factory;

  auto one = factory.CreateTag("Name1", "Value1");
  auto two = factory.CreateTag("Name2", "Value2");
  auto three = factory.CreateTag("Name3", "Value3");

  REQUIRE("Name1" == one.Name());
  REQUIRE("Value1" == one.Value());
  REQUIRE("Name2" == two.Name());
  REQUIRE("Value2" == two.Value());
  REQUIRE("Name3" == three.Name());
  REQUIRE("Value3" == three.Value());
}

TEST_CASE("Tag Large Test")
{
  tagset::TagFactory factory;

  std::vector<tagset::Tag> tags;
  std::vector<std::pair<std::string, std::string>> randomTagStrings;
  std::srand((unsigned int)std::time(nullptr));// use current time as seed for random generator
  const auto max = 1000;
  auto count = max;
  while (count > 0)
  {
    int randomValue = std::rand();
    auto nameStr = fmt::format("Name-{}", randomValue);
    auto valueStr = fmt::format("Value-{}", randomValue);

    tags.push_back(factory.CreateTag(nameStr, valueStr));
    randomTagStrings.push_back(std::make_pair(nameStr, valueStr));
    count--;
  }

  for (std::size_t idx = 0; idx < max; idx++)
  {
    const auto& tag = tags[idx];
    const auto& tagStrings = randomTagStrings[idx];
    REQUIRE(tag.Name() == tagStrings.first);
    REQUIRE(tag.Value() == tagStrings.second);
  }
}