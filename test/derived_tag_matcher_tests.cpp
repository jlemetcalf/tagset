#include <catch2/catch.hpp>

#include <cstdlib>
#include <ctime>
#include <fmt/core.h>

#include "derived_tag_matcher.hpp"
#include "tag_factory.hpp"

auto GenerateTagset1(core::TagFactory& tagFactory)
{
  auto tag1 = tagFactory.CreateTag("TagName1", "TagValue1");
  auto tag2 = tagFactory.CreateTag("TagName2", "TagValue2");
  core::TagSet tagSet;
  tagSet.insert(tag1);
  tagSet.insert(tag2);
  return tagSet;
}

TEST_CASE("Derived Tag with match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  REQUIRE(core::DerivedTagMatcher::Match("TagName1 == \"TagValue1\"", tagSet, tagFactory.GetUniqueTagNames()));
}

TEST_CASE("Derived Tag with no match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  REQUIRE(!core::DerivedTagMatcher::Match("TagName1 == \"TagValue\"", tagSet, tagFactory.GetUniqueTagNames()));
}

TEST_CASE("Derived Tag with no match for missing tag")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  // The missing tag
  [[maybe_unused]] auto tag = tagFactory.CreateTag("TagName3", "TagValue3");
  // Check it has been created with the default of empty string
  REQUIRE(core::DerivedTagMatcher::Match("TagName3 == \"\"", tagSet, tagFactory.GetUniqueTagNames()));
  // Now check it doesn't match the value as it should not exist in the tag set
  REQUIRE(!core::DerivedTagMatcher::Match("TagName3 == \"TagValue3\"", tagSet, tagFactory.GetUniqueTagNames()));
}

auto GenerateTagset2(core::TagFactory& tagFactory)
{
  auto tag1 = tagFactory.CreateTag("TagName1", "TagValue1");
  auto tag2 = tagFactory.CreateTag("TagName2", "TagValue2");
  auto tag3 = tagFactory.CreateTag("TagName3", "TagValue3");
  auto tag4 = tagFactory.CreateTag("TagName4", "TagValue4");
  core::TagSet tagSet;
  tagSet.insert(tag1);
  tagSet.insert(tag2);
  tagSet.insert(tag3);
  tagSet.insert(tag4);
  return tagSet;
}

TEST_CASE("Chaiscript Derived Tag more complex with match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset2(tagFactory);
  REQUIRE(core::DerivedTagMatcher::Match("TagName1 == \"TagValue1\" && TagName3 == \"TagValue3\"", tagSet, tagFactory.GetUniqueTagNames()));
}

TEST_CASE("Chaiscript Derived Tag more complex with no match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset2(tagFactory);
  REQUIRE(!core::DerivedTagMatcher::Match("TagName1 == \"TagValue1\" && TagName3 == \"TagValue\"", tagSet, tagFactory.GetUniqueTagNames()));
}