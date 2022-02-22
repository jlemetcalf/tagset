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

  std::vector<TEntityIdType> FindEntities(const TagSet& tagSet)
  {
    std::vector<TEntityIdType> entities;
    std::for_each(mEntities.begin(), mEntities.end(), [&entities, &tagSet](const auto& pair) {
      auto checkTag = [&pair](const auto& tag) {
        const auto It = pair.second.find(tag);
        return It != pair.second.end();
      };
      if (std::all_of(tagSet.begin(), tagSet.end(), checkTag))
      {
        entities.push_back(pair.first);
      }
    });
    return entities;
  }


protected:

private:
  std::unordered_map<TEntityIdType, TagSet> mEntities;
};

}// namespace core