#pragma once

#include <string>

#include "tag.hpp"
#include "string_cache.hpp"

namespace core {

class TagFactory
{
public:
  using TagMap = std::unordered_map<std::string, std::unordered_set<Tag, TagHash>>;
  TagFactory() = default;
  
  // Adding in nodiscard to make sure tags are not accidentally created and just fill up the cache.
  [[nodiscard]] Tag CreateTag(const std::string& name, const std::string& value)
  {
    auto cachedName = mCache.GetOrCache(name);
    auto cachedValue = mCache.GetOrCache(value);
    auto tag = Tag(cachedName->get(), cachedValue->get());
    AddUniqueTag(name, tag);
    return tag;
  }

  const TagMap& GetUniqueTags() const
  {
    return mUniqueTags;
  }

protected:
  void AddUniqueTag(const std::string& name, const Tag& tag)
  {
    auto findIt = mUniqueTags.find(name);
    if (findIt == mUniqueTags.end())
    {
      findIt = mUniqueTags.insert(std::make_pair(name, std::unordered_set<Tag, TagHash>())).first;
    }
    findIt->second.insert(tag);
  }

private:
  StringCache mCache;
  TagMap mUniqueTags;
};

}// namespace core