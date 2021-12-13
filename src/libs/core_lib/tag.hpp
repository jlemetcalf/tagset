#pragma once

#include <string>
#include <functional>

namespace core {

class TagFactory;

class Tag
{
public:
  Tag() = delete;

  const std::string& Name() const { return mName; }
  const std::string& Value() const { return mValue; }

  bool operator==(const Tag& rhs) const
  {
    // As Tags can only be generated from the string cache we can check addresses
    return (&mName == &rhs.mName) && 
           (&mValue == &rhs.mValue); 
  }

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

struct TagHash
{
    std::size_t operator()(const Tag& tag) const noexcept
    {
        std::size_t h1 = std::hash<std::string>{}(tag.Name());
        std::size_t h2 = std::hash<std::string>{}(tag.Value());
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

}// namespace core