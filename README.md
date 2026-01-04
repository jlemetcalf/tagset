# TagSet

![Build Status](https://github.com/jlemetcalf/tagset/actions/workflows/build_cmake.yml/badge.svg)
[![codecov](https://codecov.io/gh/jlemetcalf/tagset/branch/master/graph/badge.svg?token=NLHX9AD6CA)](https://codecov.io/gh/jlemetcalf/tagset)
[![experimental](http://badges.github.io/stability-badges/dist/experimental.svg)](http://github.com/badges/stability-badges)

Simple project presented as is in an experimental state.  Intent is to create a library that is able to tag entities with name/value pairs.  The library is most useful and efficient where a set of entities has alot of tags in common.  All names and values are stored in a string cache to reduce memory consumption.

## Example Usage

```
#include "tag_factory.hpp"
#include "entity_store.hpp"
#include "utils.hpp"

int main(int argc, char* argv[])
{
  tagset::TagFactory tagFactory;
  tagset::EntityStore<u_int64_t> store{ tagFactory };

  auto tagSet1 = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue1" }, { "TagName2", "TagValue2" } });
  auto tagSet2 = tagset::GenerateTagSet(tagFactory, { { "TagName3", "TagValue3" }, { "TagName4", "TagValue4" } });

  store.Add(1, tagSet1);
  store.Add(2, tagSet1);
  store.Add(3, tagSet2);

  const auto entities1 = store.FindEntities(tagSet1);
}
```

It is also possible to create derived tags:

```
  const auto includedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName3", "TagValue3" } });
  const auto excludedTagSet = tagset::GenerateTagSet(tagFactory, { { "TagName1", "TagValue" } });
  tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, excludedTagSet };
  store.AddDerivedTagDefinition(std::move(derivedTagDefinition));

## Build

```
  conan profile detect --force
  conan install . --output-folder=../build --build=missing
  make --build ../build --config Release --target all --
