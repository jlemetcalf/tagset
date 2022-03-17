#pragma once

#include <string>
#include <charconv>
#include <iostream>
#include <sstream>
#include <variant>

#define CHAISCRIPT_NO_THREADS
#include <chaiscript/chaiscript.hpp>

#include "tag.hpp"

namespace core {

class DerivedTagMatcher
{
public:
  DerivedTagMatcher() = default;

  static bool Match(const std::string& chaiScript, const TagSet& tagset, const std::unordered_set<std::string> uniqueTagNames)
  {
    DerivedTagMatcher matcher;
    matcher.Init(uniqueTagNames);
    return matcher.Match(chaiScript, tagset);
  }
  // static bool Match(const std::string& chaiScript, const TagSet& tagset, const std::unordered_set<std::string> uniqueTagNames)
  // {
  //   chaiscript::ChaiScript chai;

  //   // Add in all unique tags as blank to cover missing tags causing exception
  //   std::for_each(uniqueTagNames.begin(), uniqueTagNames.end(), [&chai](const auto& tagName) 
  //   { 
  //     chai.add(chaiscript::var(std::string("")), tagName);  
  //   });

  //   // Add in all tags
  //   std::for_each(tagset.begin(), tagset.end(), [&chai](const auto& tag) 
  //   { 
  //     chai.add(chaiscript::var(tag.Value()), tag.Name());  
  //   });

  //   return chai.eval<bool>(chaiScript);
  // }

  void Init(const std::unordered_set<std::string> uniqueTagNames)
  {
    mChai = std::make_unique<chaiscript::ChaiScript>();
    // Add in all unique tags as blank to cover missing tags causing exception
    std::for_each(uniqueTagNames.begin(), uniqueTagNames.end(), [&](const auto& tagName) 
    { 
      mChai->add_global(chaiscript::var(std::string("")), tagName);  
    });
  }

  bool Match(const std::string& chaiScript, const TagSet& tagset)
  {
    const auto state = mChai->get_state();

    // Add in all tags
    std::for_each(tagset.begin(), tagset.end(), [&](const auto& tag) 
    { 
      mChai->add(chaiscript::var(tag.Value()), tag.Name());  
    });

    const auto result = mChai->eval<bool>(chaiScript);

    mChai->set_state(state);

    return result;
  }
private:
  std::unique_ptr<chaiscript::ChaiScript> mChai;
};

}// namespace core