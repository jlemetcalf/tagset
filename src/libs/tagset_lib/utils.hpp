#pragma once

#include "tag_factory.hpp"

namespace tagset {

inline TagSet GenerateTagSet(TagFactory& tagFactory, std::initializer_list<std::pair<std::string, std::string>> tagNameValuePairs)
{
  TagSet tagSet;
  for (auto tagNameValuePair : tagNameValuePairs)
  {
    auto tag = tagFactory.CreateTag(tagNameValuePair.first, tagNameValuePair.second);
    tagSet.insert(tag);
  }
  return tagSet;
}

}// namespace tagset