#include <catch2/catch.hpp>

#include <cstdlib>
#include <ctime>
#include <fmt/core.h>

#include "derived_tag_matcher.hpp"
#include "tag_factory.hpp"
#include "utils.hpp"

auto GenerateTagset1(core::TagFactory& tagFactory)
{
  return core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}, 
      {"TagName2", "TagValue2"}
    });
}

TEST_CASE("Derived Tag with included match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto includedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", includedTagSet, {}};
  REQUIRE(core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with no included match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto includedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", includedTagSet, {}};
  REQUIRE(!core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with no included match for missing tag")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto includedTagSet = core::GenerateTagSet(tagFactory, {{"TagName3", "TagValue3"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", includedTagSet, {}};
  // Check it doesn't match the value as it should not exist in the tag set
  REQUIRE(!core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with excluded match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto excludedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", {}, excludedTagSet};
  REQUIRE(core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with no excluded match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto excludedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", {}, excludedTagSet};
  REQUIRE(!core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with excluded match for missing tag")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto excludedTagSet = core::GenerateTagSet(tagFactory, {{"TagName3", "TagValue3"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", {}, excludedTagSet};
  // Check excluded match as it should not exist in the tag set
  REQUIRE(core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

auto GenerateTagset2(core::TagFactory& tagFactory)
{
  return core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}, 
      {"TagName2", "TagValue2"},
      {"TagName3", "TagValue3"},
      {"TagName4", "TagValue4"}
    });
}

TEST_CASE("Derived Tag more complex with included match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}, {"TagName3", "TagValue3"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", includedTagSet, {}};
  REQUIRE(core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag more complex with no included match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}, {"TagName3", "TagValue"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", includedTagSet, {}};
  REQUIRE(!core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag more complex with included and excluded match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}, {"TagName3", "TagValue3"}});
  const auto excludedTagSet = core::GenerateTagSet(tagFactory, {{"TagName6", "TagValue6"}, {"TagName7", "TagValue7"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", includedTagSet, {}};
  REQUIRE(core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag more complex with included and excluded no match")
{
  core::TagFactory tagFactory;
  core::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = core::GenerateTagSet(tagFactory, {{"TagName1", "TagValue1"}, {"TagName6", "TagValue6"}});
  const auto excludedTagSet = core::GenerateTagSet(tagFactory, {{"TagName4", "TagValue4"}, {"TagName7", "TagValue7"}});
  core::DerivedTagDefinition derivedTagDefinition{"DerivedTagName1", "DerivedTagValue1", includedTagSet, {}};
  REQUIRE(!core::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}