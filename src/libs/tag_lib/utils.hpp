#pragma once

#include "tag_factory.hpp"

namespace core {

inline core::TagSet GenerateTagSet(core::TagFactory& tagFactory, std::initializer_list<std::pair<std::string, std::string>> tagNameValuePairs)
{
  core::TagSet tagSet;
  for (auto tagNameValuePair : tagNameValuePairs)
  {
    auto tag = tagFactory.CreateTag(tagNameValuePair.first, tagNameValuePair.second);
    tagSet.insert(tag);
  }
  return tagSet;
}

}// namespace core