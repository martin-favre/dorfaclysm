#pragma once
#include <string>
#include <sstream>
#define LOG(string) Logging::log(std::stringstream() << string)

class Logging {
 public:
  static void log(const std::string& text);
  static void log(const std::ostream& text);
};