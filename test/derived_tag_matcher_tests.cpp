#include <catch2/catch.hpp>

#include <cstdlib>
#include <ctime>
#include <fmt/core.h>

#include "derived_tag_matcher.hpp"
#include "tag_factory.hpp"
#include "utils.hpp"

auto GenerateTagset1(tagset::TagFactory& tagFactory)
{
  return tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName2", "TagValue2" } });
}

TEST_CASE("Derived Tag with included match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  REQUIRE(tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with no included match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  REQUIRE(!tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with no included match for missing tag")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName3", "TagValue3" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  // Check it doesn't match the value as it should not exist in the tag set
  REQUIRE(!tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with excluded match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto excludedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", {}, excludedTagSet };
  REQUIRE(tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with no excluded match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto excludedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", {}, excludedTagSet };
  REQUIRE(!tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag with excluded match for missing tag")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset1(tagFactory);
  const auto excludedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName3", "TagValue3" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", {}, excludedTagSet };
  // Check excluded match as it should not exist in the tag set
  REQUIRE(tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

auto GenerateTagset2(tagset::TagFactory& tagFactory)
{
  return tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName2", "TagValue2" }, { "TagName3", "TagValue3" }, { "TagName4", "TagValue4" } });
}

TEST_CASE("Derived Tag more complex with included match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName3", "TagValue3" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  REQUIRE(tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag more complex with no included match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName3", "TagValue" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  REQUIRE(!tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag more complex with included and excluded match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName3", "TagValue3" } });
  const auto excludedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName6", "TagValue6" }, { "TagName7", "TagValue7" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  REQUIRE(tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}

TEST_CASE("Derived Tag more complex with included and excluded no match")
{
  tagset::TagFactory tagFactory;
  tagset::TagSet tagSet = GenerateTagset2(tagFactory);
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName6", "TagValue6" } });
  const auto excludedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName4", "TagValue4" }, { "TagName7", "TagValue7" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
  REQUIRE(!tagset::DerivedTagMatcher::Match(derivedTagDefinition, tagSet));
}