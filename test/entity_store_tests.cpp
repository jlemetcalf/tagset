#include <catch2/catch.hpp>

#include <cstdlib>
#include <ctime>

#include <fmt/core.h>

#include "tag_factory.hpp"
#include "entity_store.hpp"
#include "utils.hpp"

TEST_CASE("Entity Store Simple Load")
{
  core::TagFactory tagFactory;
  core::EntityStore<u_int64_t> store{ tagFactory };

  auto tag1 = tagFactory.CreateTag("TagName1", "TagValue1");
  core::TagSet tagSet;
  tagSet.insert(tag1);
  store.Add(1, tagSet);

  const auto storedTagSetOptional = store.GetTags(1);
  REQUIRE(storedTagSetOptional != std::nullopt);
  const auto storedTagSet = storedTagSetOptional->get();
  REQUIRE("TagName1" == storedTagSet.begin()->Name());
  REQUIRE("TagValue1" == storedTagSet.begin()->Value());
}

TEST_CASE("Entity Store Lookup")
{
  core::TagFactory tagFactory;
  core::EntityStore<u_int64_t> store{ tagFactory };

  auto tagSet1 = core::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName2", "TagValue2" } });
  auto tagSet2 = core::GenerateTagSet(tagFactory, { { "TagName3", "TagValue3" }, { "TagName4", "TagValue4" } });

  store.Add(1, tagSet1);
  store.Add(2, tagSet1);
  store.Add(3, tagSet2);

  const auto entities1 = store.FindEntities(tagSet1);
  REQUIRE(entities1.size() == 2);
  REQUIRE(std::find(entities1.begin(), entities1.end(), 1) != entities1.end());
  REQUIRE(std::find(entities1.begin(), entities1.end(), 2) != entities1.end());

  const auto entities2 = store.FindEntities(tagSet2);
  REQUIRE(entities2.size() == 1);
  REQUIRE(std::find(entities2.begin(), entities2.end(), 3) != entities2.end());
}

TEST_CASE("Entity Store Get Unique Tags")
{
  core::TagFactory tagFactory;
  core::EntityStore<u_int64_t> store{ tagFactory };

  [[maybe_unused]] auto tag1 = tagFactory.CreateTag("TagName1", "TagValue1-1");
  [[maybe_unused]] auto tag2 = tagFactory.CreateTag("TagName1", "TagValue1-2");
  [[maybe_unused]] auto tag3 = tagFactory.CreateTag("TagName2", "TagValue2-1");
  [[maybe_unused]] auto tag4 = tagFactory.CreateTag("TagName2", "TagValue2-2");

  [[maybe_unused]] auto duplicateTag1 = tagFactory.CreateTag("TagName1", "TagValue1-1");
  [[maybe_unused]] auto duplicateTag2 = tagFactory.CreateTag("TagName1", "TagValue1-2");
  [[maybe_unused]] auto duplicateTag3 = tagFactory.CreateTag("TagName2", "TagValue2-1");
  [[maybe_unused]] auto duplicateTag4 = tagFactory.CreateTag("TagName2", "TagValue2-2");

  const auto& uniqueTags = tagFactory.GetUniqueTags();

  REQUIRE(uniqueTags.size() == 2);

  auto checkTagsLambda = [&uniqueTags](const std::string_view tagName, std::unordered_set<std::string> values) -> void {
    const auto tagNameIt = uniqueTags.find(std::string(tagName));
    REQUIRE(tagNameIt != uniqueTags.end());
    REQUIRE(tagNameIt->second.size() == values.size());
    std::for_each(tagNameIt->second.begin(), tagNameIt->second.end(), [&values](const core::Tag& tag) {
      values.erase(tag.Value());
    });
    REQUIRE(values.size() == 0);
  };

  checkTagsLambda("TagName1", { "TagValue1-1", "TagValue1-2" });
  checkTagsLambda("TagName2", { "TagValue2-1", "TagValue2-2" });

  const auto& uniqueTagNames = tagFactory.GetUniqueTagNames();
  REQUIRE(uniqueTagNames.size() == 2);
  REQUIRE(uniqueTagNames.find("TagName1") != uniqueTagNames.end());
  REQUIRE(uniqueTagNames.find("TagName2") != uniqueTagNames.end());
}

TEST_CASE("Entity Store Lookup for derived tag")
{
  core::TagFactory tagFactory;
  core::EntityStore<u_int64_t> store{ tagFactory };

  const auto includedTagSet = core::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" } });
  core::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  store.AddDerivedTagDefinition(std::move(derivedTagDefinition));

  auto tagSet1 = core::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName2", "TagValue2" } });
  auto tagSet2 = core::GenerateTagSet(tagFactory, { { "TagName3", "TagValue3" }, { "TagName4", "TagValue4" } });

  store.Add(1, tagSet1);
  store.Add(2, tagSet1);
  store.Add(3, tagSet2);

  auto tagThatHasBeenDerived = tagFactory.CreateTag("DerivedTagName1", "DerivedTagValue1");
  core::TagSet derivedTagSet;
  derivedTagSet.insert(tagThatHasBeenDerived);

  const auto entities = store.FindEntities(derivedTagSet);
  REQUIRE(entities.size() == 2);
  REQUIRE(std::find(entities.begin(), entities.end(), 1) != entities.end());
  REQUIRE(std::find(entities.begin(), entities.end(), 2) != entities.end());
}

TEST_CASE("Entity Store Lookup for derived tag and then remove")
{
  core::TagFactory tagFactory;
  core::EntityStore<u_int64_t> store{ tagFactory };

  const auto includedTagSet = core::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" } });
  core::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  store.AddDerivedTagDefinition(std::move(derivedTagDefinition));

  auto tagSet1 = core::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName2", "TagValue2" } });
  auto tagSet2 = core::GenerateTagSet(tagFactory, { { "TagName3", "TagValue3" }, { "TagName4", "TagValue4" } });

  store.Add(1, tagSet1);
  store.Add(2, tagSet1);
  store.Add(3, tagSet2);

  auto tagThatHasBeenDerived = tagFactory.CreateTag("DerivedTagName1", "DerivedTagValue1");
  core::TagSet derivedTagSet;
  derivedTagSet.insert(tagThatHasBeenDerived);

  const auto entities = store.FindEntities(derivedTagSet);
  REQUIRE(entities.size() == 2);
  REQUIRE(std::find(entities.begin(), entities.end(), 1) != entities.end());
  REQUIRE(std::find(entities.begin(), entities.end(), 2) != entities.end());

  store.RemoveDerivedTag("DerivedTagName1");
  const auto entitiesAfterRemove = store.FindEntities(derivedTagSet);
  REQUIRE(entitiesAfterRemove.size() == 0);
}