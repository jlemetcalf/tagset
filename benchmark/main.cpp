#include <benchmark/benchmark.h>
#include <optional>
#include <functional>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <memory>

#include <fmt/core.h>

#include "entity_store.hpp"
#include "tag_factory.hpp"
#include "utils.hpp"

std::string gen_random(const std::size_t len)
{
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (std::size_t i = 0; i < len; ++i)
  {
    tmp_s += alphanum[(std::size_t)rand() % (sizeof(alphanum) - 1)];
  }

  return tmp_s;
}

class EntityStoreFixture : public benchmark::Fixture
{
public:
  void SetUp(const ::benchmark::State& /*state*/) override
  {
    mTagFactory = std::make_unique<tagset::TagFactory>();
    mStore = std::make_unique<tagset::EntityStore<u_int64_t>>(*mTagFactory.get());
    GenerateTags(100000, 10);
  }

  void GenerateTags(u_int64_t count, const std::size_t tags)
  {
    while (count > 0)
    {
      tagset::TagSet tagSet;
      for (std::size_t idx = 0; idx < tags; idx++)
      {
        auto tag1 = mTagFactory->CreateTag(std::string("Tag") + gen_random(20), std::string("Value") + gen_random(20));
        tagSet.insert(tag1);
      }
      mStore->Add(count, tagSet);
      count--;
    }
  }

  void TearDown(const ::benchmark::State& /*state*/) override
  {
  }

  std::unique_ptr<tagset::TagFactory> mTagFactory;
  std::unique_ptr<tagset::EntityStore<u_int64_t>> mStore;
};

BENCHMARK_F(EntityStoreFixture, LookupTest)
(benchmark::State& state)
{
  for (auto _ : state)
  {
    // This code gets timed
    const auto storedTagSetOptional = mStore->GetTags(1);
    const auto& storedTagSet = storedTagSetOptional->get();
    [[maybe_unused]] const auto size = storedTagSet.size();
  }
}

BENCHMARK_F(EntityStoreFixture, FindEntitiesTest)
(benchmark::State& state)
{
  const auto tags = mStore->GetTags(1);
  for (auto _ : state)
  {
    // This code gets timed
    const auto entities1 = mStore->FindEntities(tags->get());
  }
}

BENCHMARK_F(EntityStoreFixture, DerivedTagGenerationTest)
(benchmark::State& state)
{
  const auto tagSetOptional = mStore->GetTags(1);
  const auto& tagSet = tagSetOptional->get();
  const auto firstTagName = tagSet.begin()->Name();
  const auto firstTagValue = tagSet.begin()->Value();
  const auto includedTagSet = tagset::GenerateTagSet(*mTagFactory, { { firstTagName, firstTagValue } });
  tagset::TagSet derivedTagSet;
  auto tagThatHasBeenDerived = mTagFactory->CreateTag("DerivedTagName1", "DerivedTagValue1");
  derivedTagSet.insert(tagThatHasBeenDerived);
  for (auto _ : state)
  {
    // This code gets timed
    tagset::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", includedTagSet, {} };
    mStore->AddDerivedTagDefinition(std::move(derivedTagDefinition));
  }
}

BENCHMARK_F(EntityStoreFixture, Generate1000Entities10Tags)
(benchmark::State& state)
{
  // Generate random tags and values upfront
  const u_int64_t count = 1000;
  const std::size_t numTags = 10;
  std::vector<std::vector<std::string>> generatedTags;
  std::vector<std::vector<std::string>> generatedValues;
  for (std::size_t entityIdx = 0; entityIdx < count; entityIdx++)
  {
    std::vector<std::string> tags;
    std::vector<std::string> values;
    for (std::size_t tagIdx = 0; tagIdx < numTags; tagIdx++)
    {
      tags.emplace_back(gen_random(20));
      values.emplace_back(gen_random(20));
    }
    generatedTags.emplace_back(std::move(tags));
    generatedValues.emplace_back(std::move(values));
  }
  for (auto _ : state)
  {
    // This code gets timed
    for (std::size_t entityIdx = 0; entityIdx < count; entityIdx++)
    {
      tagset::TagSet tagSet;
      for (std::size_t tagIdx = 0; tagIdx < numTags; tagIdx++)
      {
        auto tag1 = mTagFactory->CreateTag(std::string("Tag") + generatedTags[entityIdx][tagIdx], std::string("Value") + generatedValues[entityIdx][tagIdx]);
        tagSet.insert(tag1);
      }
      mStore->Add(count, tagSet);
    }
  }
}

// Run the benchmark
BENCHMARK_MAIN();