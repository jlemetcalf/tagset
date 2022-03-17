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

std::string gen_random(const std::size_t len) 
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (std::size_t i = 0; i < len; ++i) {
        tmp_s += alphanum[(std::size_t)rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

class EntityStoreFixture : public benchmark::Fixture 
{
public:
  void SetUp(const ::benchmark::State& /*state*/) override
  {
    mTagFactory = std::make_unique<core::TagFactory>();
    mStore = std::make_unique<core::EntityStore<u_int64_t>>(*mTagFactory.get());
    //u_int64_t count = 100000;
    u_int64_t count = 100;
    std::size_t tags = 10;
    while (count > 0)
    {
        core::TagSet tagSet;
        for (std::size_t idx = 0; idx < tags; idx++)
        {
          auto tag1 = mTagFactory->CreateTag(gen_random(20), gen_random(20));
          tagSet.insert(tag1);
        }
        mStore->Add(count, tagSet );
        count--;
    }
  }

  void TearDown(const ::benchmark::State& /*state*/) override
  {
  }

  std::unique_ptr<core::TagFactory> mTagFactory;
  std::unique_ptr<core::EntityStore<u_int64_t>> mStore;
};

BENCHMARK_F(EntityStoreFixture, LookupTest)(benchmark::State& state) 
{
  for (auto _ : state) 
  {
    // This code gets timed
    const auto storedTagSetOptional = mStore->GetTags(1);
    const auto& storedTagSet = storedTagSetOptional->get();
    [[maybe_unused]] const auto size = storedTagSet.size();
  }
}

BENCHMARK_F(EntityStoreFixture, FindEntitiesTest)(benchmark::State& state) 
{
  const auto tags = mStore->GetTags(1);
  for (auto _ : state) 
  {
    // This code gets timed
    const auto entities1 = mStore->FindEntities(tags->get());
  }
}

BENCHMARK_F(EntityStoreFixture, DerivedTagGenerationTest)(benchmark::State& state) 
{
  const auto tagSetOptional = mStore->GetTags(1);
  const auto& tagSet = tagSetOptional->get();
  const auto firstTagName = tagSet.begin()->Name();
  const auto firstTagValue = tagSet.begin()->Value();
  const auto derivedTagExpression = fmt::format("{} == \"{}\"", firstTagName, firstTagValue);
  core::TagSet derivedTagSet;
  core::DerivedTagDefinition derivedTagDefinition{ "DerivedTagName1", "DerivedTagValue1", derivedTagExpression};
  auto tagThatHasBeenDerived = mTagFactory->CreateTag("DerivedTagName1", "DerivedTagValue1");
  derivedTagSet.insert(tagThatHasBeenDerived);
  for (auto _ : state) 
  {
    // This code gets timed
    mStore->AddDerivedTagDefinition(std::move(derivedTagDefinition));
    const auto entities1 = mStore->FindEntities(derivedTagSet);
  }
}

// Run the benchmark
BENCHMARK_MAIN();