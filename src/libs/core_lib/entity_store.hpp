#pragma once

#include <unordered_set>
#include <unordered_map>
#include <optional>
#include <functional>
#include <string>

#include "tag.hpp"

namespace core {

template<typename TEntityIdType>
class EntityStore
{
public:
  using TagSet = std::unordered_set<Tag, TagHash>;
  EntityStore() = default;

  void Add(TEntityIdType entityId, TagSet& tagSet)
  {
    mEntities.insert(std::make_pair(entityId, tagSet));
  }

  void AddTags(TEntityIdType entityId, const TagSet& tagSet)
  {
    auto it = mEntities.find(entityId);
    if (it != mEntities.end())
    {
      it->second.insert(tagSet.begin(), tagSet.end());
    }
  }

  void RemoveTags(TEntityIdType entityId, const TagSet& tagSet)
  {
    auto it = mEntities.find(entityId);
    if (it != mEntities.end())
    {
      for (const auto& tag : tagSet)
      {
        it->second.erase(tag);
      }
    }
  }

  std::optional<std::reference_wrapper<const TagSet>> GetTags(TEntityIdType entityId)
  {
    auto it = mEntities.find(entityId);
    if (it != mEntities.end())
    {
      return std::optional<std::reference_wrapper<const TagSet>>{it->second};
    }
    return std::nullopt;
  }

protected:

private:
  std::unordered_map<TEntityIdType, TagSet> mEntities;
};

}// namespace core