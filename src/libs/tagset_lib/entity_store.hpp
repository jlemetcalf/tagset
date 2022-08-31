#pragma once

#include <unordered_map>
#include <optional>
#include <functional>
#include <string>

#include "absl/container/flat_hash_map.h"

#include "tag.hpp"
#include "derived_tag_definition.hpp"
#include "tag_factory.hpp"
#include "derived_tag_matcher.hpp"

namespace tagset {

template<typename TEntityIdType>
class EntityStore
{
public:
  EntityStore(TagFactory& tagFactory) : mTagFactory(tagFactory){};

  void Add(TEntityIdType entityId, TagSet& tagSet)
  {
    mEntities.insert(std::make_pair(entityId, tagSet));
    UpdateDerivedTagDefinitions(entityId);
  }

  void AddTags(TEntityIdType entityId, const TagSet& tagSet)
  {
    AddTagsInternal(entityId, tagSet);
  }

  void RemoveTags(TEntityIdType entityId, const TagSet& tagSet)
  {
    RemoveTagsInternal(entityId, tagSet);
  }

  std::optional<std::reference_wrapper<const TagSet>> GetTags(TEntityIdType entityId)
  {
    auto it = mEntities.find(entityId);
    if (it != mEntities.end())
    {
      return std::optional<std::reference_wrapper<const TagSet>>{ it->second };
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

  void AddDerivedTagDefinition(DerivedTagDefinition&& derivedTagDefinition)
  {
    auto tag = mTagFactory.CreateTag(derivedTagDefinition.Name(), derivedTagDefinition.Value());
    TagSet tagSet;
    tagSet.insert(tag);
    std::for_each(mEntities.begin(), mEntities.end(), [&](auto& entityPair) {
      if (DerivedTagMatcher::Match(derivedTagDefinition, entityPair.second))
      {
        AddTagsInternal(entityPair.first, tagSet, false);
      }
    });
    mDerivedTagDefinitions.emplace(derivedTagDefinition.Name(), std::move(derivedTagDefinition));
  }

  void RemoveDerivedTag(std::string_view name)
  {
    const auto derivedTagIt = mDerivedTagDefinitions.find(std::string(name));
    if (derivedTagIt != mDerivedTagDefinitions.end())
    {
      auto tag = mTagFactory.CreateTag(derivedTagIt->second.Name(), derivedTagIt->second.Value());
      TagSet tagSet;
      tagSet.insert(tag);
      std::for_each(mEntities.begin(), mEntities.end(), [&](auto& entityPair) {
        RemoveTagsInternal(entityPair.first, tagSet, false);
      });
      mDerivedTagDefinitions.erase(std::string(name));
    }
  }

  void UpdateDerivedTagDefinitions(TEntityIdType entityId)
  {
    const auto& tagset = GetTags(entityId);
    if (tagset)
    {
      std::for_each(mDerivedTagDefinitions.begin(), mDerivedTagDefinitions.end(), [&](const auto& derivedTagPair) {
        const auto& mDerivedTagDefinition = derivedTagPair.second;
        auto tag = mTagFactory.CreateTag(mDerivedTagDefinition.Name(), mDerivedTagDefinition.Value());
        TagSet derivedTagSet;
        derivedTagSet.insert(tag);
        if (DerivedTagMatcher::Match(mDerivedTagDefinition, *tagset))
        {
          AddTagsInternal(entityId, derivedTagSet, false);
        } else
        {
          RemoveTagsInternal(entityId, derivedTagSet, false);
        }
      });
    }
  }

protected:
  void AddTagsInternal(TEntityIdType entityId, const TagSet& tagSet, bool updateDerivedTags = true)
  {
    auto it = mEntities.find(entityId);
    if (it != mEntities.end())
    {
      it->second.insert(tagSet.begin(), tagSet.end());
    }
    if (updateDerivedTags)
    {
      UpdateDerivedTagDefinitions(entityId);
    }
  }

  void RemoveTagsInternal(TEntityIdType entityId, const TagSet& tagSet, bool updateDerivedTags = true)
  {
    auto it = mEntities.find(entityId);
    if (it != mEntities.end())
    {
      for (const auto& tag : tagSet)
      {
        it->second.erase(tag);
      }
    }
    if (updateDerivedTags)
    {
      UpdateDerivedTagDefinitions(entityId);
    }
  }

private:
  TagFactory& mTagFactory;
  absl::flat_hash_map<TEntityIdType, TagSet> mEntities;
  absl::flat_hash_map<std::string, DerivedTagDefinition> mDerivedTagDefinitions;
};

}// namespace tagset