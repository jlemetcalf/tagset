#pragma once

#include <vector>
#include <string_view>
#include <map>

#include <docopt/docopt.h>

namespace core {

class Application
{
public:
  Application(std::string_view applicationName);
  void SetupLogging(std::map<std::string, docopt::value>& args);

protected:
  static std::string CurrentDateTime();

private:
  std::string mApplicationName;
};

}// namespace core