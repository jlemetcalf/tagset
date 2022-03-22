#pragma once

#include <string>

namespace core {

class DerivedTagDefinition
{
public:
  DerivedTagDefinition() = delete;
  DerivedTagDefinition(std::string_view name, std::string_view value, const TagSet& includedTags, const TagSet& excludedTags)
    : mName(name), mValue(value), mIncludedTags(includedTags), mExcludedTags(excludedTags){};

  const std::string& Name() const { return mName; }
  const std::string& Value() const { return mValue; }
  const TagSet& IncludedTags() const { return mIncludedTags; }
  const TagSet& ExcludedTags() const { return mExcludedTags; }

  bool operator==(const DerivedTagDefinition& rhs) const
  {
    return (mName == rhs.mName) && (mValue == rhs.mValue) && (mIncludedTags == rhs.mExcludedTags);
  }

private:
  std::string mName;
  std::string mValue;
  TagSet mIncludedTags;
  TagSet mExcludedTags;
};

}// namespace core