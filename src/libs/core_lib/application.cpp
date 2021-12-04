#include "application.hpp"

#include <iomanip>
#include <string>

#include <fmt/core.h>
#include <fmt/core.h>
#include <fmt/chrono.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks-inl.h>

using namespace core;

Application::Application(std::string_view applicationName)
: mApplicationName( applicationName )
{
}

std::string Application::CurrentDateTime()
{
  std::time_t tt = std::time(nullptr);
  std::tm *tm = std::localtime(&tt);
  return fmt::format("{:%Y%m%d-%H%M%S}", *tm);
}

void Application::SetupLogging(std::map<std::string, docopt::value>& args)
{
  std::vector<spdlog::sink_ptr> sinks;
  if (args["-c"].asBool()) {
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
  }
  sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("./logs/{}_{}.log", CurrentDateTime(), mApplicationName)));
  auto combined_logger = std::make_shared<spdlog::logger>("default", begin(sinks), end(sinks));
  spdlog::register_logger(combined_logger);
  spdlog::set_default_logger(combined_logger);
  spdlog::flush_every(std::chrono::seconds(1));
  if (args["-l"].asString() == std::string_view("INFO")) {
    spdlog::set_level(spdlog::level::info);
  } else if (args["-l"].asString() == std::string_view("DEBUG")) {
    spdlog::set_level(spdlog::level::debug);
  }
}