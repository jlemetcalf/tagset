#pragma once

#include <unordered_set>
#include <optional>
#include <functional>
#include <string>

namespace core {

class StringCache
{
public:
  StringCache() = default;
  std::optional<std::reference_wrapper<const std::string>> GetOrCache(const std::string& str);

protected:
  std::optional<std::reference_wrapper<const std::string>> Cache(const std::string& str);
  std::optional<std::reference_wrapper<const std::string>> Get(const std::string& str) const;

private:
  std::unordered_set<std::string> mStrings;
};

}// namespace core