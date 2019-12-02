#pragma once
#include <string>
class Logging {
 public:
  static void log(const std::string& text);
  static void log(const std::stringstream& text);
};