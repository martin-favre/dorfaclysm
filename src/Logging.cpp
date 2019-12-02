#include "Logging.h"

#include <iostream>
#include <sstream>
void Logging::log(const std::string& text) { std::cout << text << "\n"; }

void Logging::log(const std::stringstream& text) { Logging::log(text.str()); }