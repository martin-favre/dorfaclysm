#include "Logging.h"

#include <iostream>
#include <sstream>
#include <ctime>  
#include <chrono> 
#include <iomanip>
void Logging::log(const std::string& text) {
#ifdef DEBUG
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::cout <<   std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << ": "<< text << "\n";
#endif
}

void Logging::log(const std::ostream& text) {
  std::stringstream ss;
  ss << text.rdbuf();
  Logging::log(ss.str());
}
