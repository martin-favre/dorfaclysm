#include "Logging.h"

#include <iostream>
#include <sstream>
void Logging::log(const std::string& text) { std::cout << text << "\n"; }

void Logging::log(const std::ostream& text) {
  std::stringstream ss;
  ss << text.rdbuf();
  Logging::log(ss.str());
}
