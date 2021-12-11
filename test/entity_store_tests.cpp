#include <catch2/catch.hpp>

#include <cstdlib>
#include <ctime>
#include <fmt/core.h>

#include "tag_factory.hpp"
#include "entity_store.hpp"

TEST_CASE("Entity Store Simple Load")
{
  core::TagFactory tagFactory;
  core::EntityStore<u_int64_t> store;

  auto tag1 = tagFactory.CreateTag("TagName1", "TagValue1");
  core::EntityStore<u_int64_t>::TagSet tagSet;
  tagSet.insert(tag1);
  store.Add(1, tagSet );

  const auto storedTagSetOptional = store.GetTags(1);
  REQUIRE(storedTagSetOptional != std::nullopt);
  const auto storedTagSet = storedTagSetOptional->get();
  REQUIRE("TagName1" == storedTagSet.begin()->Name());
  REQUIRE("TagValue1" == storedTagSet.begin()->Value());
  // REQUIRE("One" == one->get());
  // REQUIRE("Two" == two->get());
  // REQUIRE("Three" == three->get());
}