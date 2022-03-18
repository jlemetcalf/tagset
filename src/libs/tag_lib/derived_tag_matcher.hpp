#pragma once

#include <string>
#include <charconv>
#include <iostream>
#include <sstream>
#include <variant>

#include "tag.hpp"
#include "derived_tag_definition.hpp"

namespace core {

class DerivedTagMatcher
{
public:
  DerivedTagMatcher() = delete;

  static bool Match(const DerivedTagDefinition& derivedTagDefinition, const TagSet& tagset)
  {
    const auto& includedTags = derivedTagDefinition.IncludedTags();
    const bool foundAllIncludedTags = std::all_of(includedTags.begin(), includedTags.end(), [&tagset](const auto tag){
      return tagset.find(tag) != tagset.end();
    });
    const auto& excludedTags = derivedTagDefinition.ExcludedTags();
    const bool foundAnyExcludedTags = std::any_of(excludedTags.begin(), excludedTags.end(), [&tagset](const auto tag){
      return tagset.find(tag) != tagset.end();
    });
    return foundAllIncludedTags && !foundAnyExcludedTags;
  }
};

}// namespace core