#pragma once

#include <string>

#include "tag.hpp"
#include "string_cache.hpp"

namespace core {

class TagFactory
{
public:
  TagFactory() = default;
  Tag CreateTag(const std::string& name, const std::string& value)
  {
    return Tag(mCache.GetOrCache(name)->get(), mCache.GetOrCache(value)->get());
  }
protected:
  StringCache mCache;
};

}// namespace core