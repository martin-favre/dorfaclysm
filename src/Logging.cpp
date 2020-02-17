#include "Logging.h"

#include <iostream>
#include <sstream>
void Logging::log(const std::string& text) {
#ifdef DEBUG
  std::cout << text << "\n";
#endif
}

void Logging::log(const std::ostream& text) {
  std::stringstream ss;
  ss << text.rdbuf();
  Logging::log(ss.str());
}
