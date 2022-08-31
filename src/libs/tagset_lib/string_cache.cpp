#include "string_cache.hpp"

using namespace tagset;

std::optional<std::reference_wrapper<const std::string>> StringCache::Cache(const std::string& str)
{
  return std::optional<std::reference_wrapper<const std::string>>{ *mStrings.insert(str).first };
}

std::optional<std::reference_wrapper<const std::string>> StringCache::Get(const std::string& str) const
{
  const auto it = mStrings.find(str);
  if (it != mStrings.end())
  {
    return std::optional<std::reference_wrapper<const std::string>>{ *it };
  }
  return std::nullopt;
}

std::optional<std::reference_wrapper<const std::string>> StringCache::GetOrCache(const std::string& str)
{
  auto value = Get(str);
  if (!value)
  {
    value = Cache(str);
  }
  return value;
}