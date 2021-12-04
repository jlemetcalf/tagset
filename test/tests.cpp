#include <catch2/catch.hpp>

#include "types.hpp"

TEST_CASE("JSON is loaded")
{
  /*const std::string jsonData = R"--({"Meta Data":{"1. Information":"Daily Prices (open, high, low, close) and Volumes","2. Symbol":"IBM","3. Last Refreshed":"2021-08-04","4. Output Size":"Compact","5. Time Zone":"US/Eastern"},"Time Series (Daily)":{"2021-03-15":{"1. open":"127.7700","2. high":"128.7500","3. low":"127.5401","4. close":"128.5800","5. volume":"3421281"},"2021-03-16":{"1. open":"128.2800","2. high":"128.5200","3. low":"127.3400","4. close":"128.2400","5. volume":"4653178"},"2021-03-17":{"1. open":"128.4600","2. high":"129.4900","3. low":"127.4900","4. close":"129.0300","5. volume":"4291351"}}})--";
  tick_data::TickDataAlphaVantage tickData;
  std::vector<core_data::DailyData> dailyData;
  tickData.Load(jsonData, dailyData);
  REQUIRE(dailyData.size() == 3);

  auto checkPrices = [] (core_data::DailyData& data, const core_data::Date& dt, double open, double high, double low, double close, int64_t volume){
    REQUIRE(data.Dt.Year == dt.Year);
    REQUIRE(data.Dt.Month == dt.Month);
    REQUIRE(data.Dt.Day == dt.Day);
    REQUIRE(data.Open == open);
    REQUIRE(data.High == high);
    REQUIRE(data.Low == low);
    REQUIRE(data.Close == close);
    REQUIRE(data.Volume == volume);
  };

  checkPrices(dailyData[0], core_data::Date{2021, 03, 15}, 127.77, 128.75, 127.5401, 128.58, 3421281);
  checkPrices(dailyData[1], core_data::Date{2021, 03, 16}, 128.28, 128.52, 127.34, 128.24, 4653178);
  checkPrices(dailyData[2], core_data::Date{2021, 03, 17}, 128.46, 129.49, 127.49, 129.03, 4291351);*/
}
