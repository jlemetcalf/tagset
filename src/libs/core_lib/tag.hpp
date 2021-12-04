#pragma once

#include <string>

namespace core {

class TagFactory;

class Tag
{
public:
  Tag() = delete;

  const std::string& Name() const { return mName; }
  const std::string& Value() const { return mValue; }

protected:
  Tag(const std::string& name, const std::string& value)
  : mName(name)
  , mValue(value)
  {

  }
  friend TagFactory;

private:
  const std::string& mName;
  const std::string& mValue;
};

}// namespace core