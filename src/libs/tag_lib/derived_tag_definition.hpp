#pragma once

#include <string>

namespace core {

class DerivedTagDefinition
{
public:
  DerivedTagDefinition() = delete;
  DerivedTagDefinition(std::string_view name, std::string_view value, std::string_view matchingExpression) 
  : mName(name), mValue(value), mMatchingExpression(matchingExpression) {};

  const std::string& Name() const { return mName; }
  const std::string& Value() const { return mValue; }
  const std::string& MatchingExpression() const { return mMatchingExpression; }

  bool operator==(const DerivedTagDefinition& rhs) const
  {
    return (mName == rhs.mName) &&
           (mValue == rhs.mValue) &&
           (mMatchingExpression == rhs.mMatchingExpression); 
  }

private:
  std::string mName;
  std::string mValue;
  std::string mMatchingExpression;
};

}// namespace core