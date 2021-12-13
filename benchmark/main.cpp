#include <benchmark/benchmark.h>
#include <optional>
#include <functional>
#include <ctime>
#include <iostream>
#include <unistd.h>

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
  void SetUp(const ::benchmark::State& /*state*/) 
  {
    u_int64_t count = 100000;
    std::size_t tags = 10;
    while (count > 0)
    {
        core::EntityStore<u_int64_t>::TagSet tagSet;
        for (std::size_t idx = 0; idx < tags; idx++)
        {
          auto tag1 = mTagFactory.CreateTag(gen_random(20), gen_random(20));
          tagSet.insert(tag1);
        }
        mStore.Add(count, tagSet );
        count--;
    }
  }

  void TearDown(const ::benchmark::State& /*state*/) {
  }

  core::TagFactory mTagFactory;
  core::EntityStore<u_int64_t> mStore;
};

BENCHMARK_F(EntityStoreFixture, LookupTest)(benchmark::State& state) 
{
  for (auto _ : state) 
  {
    // This code gets timed
    const auto storedTagSetOptional = mStore.GetTags(1);
    const auto& storedTagSet = storedTagSetOptional->get();
    [[maybe_unused]] const auto size = storedTagSet.size();
  }
}

BENCHMARK_F(EntityStoreFixture, FindEntitiesTest)(benchmark::State& state) 
{
  const auto tags = mStore.GetTags(1);
  for (auto _ : state) 
  {
    // This code gets timed
    const auto entities1 = mStore.FindEntities(tags->get());
  }
}

// Run the benchmark
BENCHMARK_MAIN();