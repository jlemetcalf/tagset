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
}

TEST_CASE("Entity Store Lookup")
{
  core::TagFactory tagFactory;
  core::EntityStore<u_int64_t> store;

  auto tag1 = tagFactory.CreateTag("TagName1", "TagValue1");
  auto tag2 = tagFactory.CreateTag("TagName2", "TagValue2");
  core::EntityStore<u_int64_t>::TagSet tagSet1;
  tagSet1.insert(tag1);
  tagSet1.insert(tag2);

  auto tag3 = tagFactory.CreateTag("TagName3", "TagValue3");
  auto tag4 = tagFactory.CreateTag("TagName4", "TagValue4");
  core::EntityStore<u_int64_t>::TagSet tagSet2;
  tagSet2.insert(tag3);
  tagSet2.insert(tag4);

  store.Add(1, tagSet1 );
  store.Add(2, tagSet1 );
  store.Add(3, tagSet2 );

  const auto entities1 = store.FindEntities(tagSet1);
  REQUIRE(entities1.size() == 2);
  REQUIRE(std::find(entities1.begin(), entities1.end(), 1) != entities1.end());
  REQUIRE(std::find(entities1.begin(), entities1.end(), 2) != entities1.end());

  const auto entities2 = store.FindEntities(tagSet2);
  REQUIRE(entities2.size() == 1);
  REQUIRE(std::find(entities2.begin(), entities2.end(), 3) != entities2.end());
}