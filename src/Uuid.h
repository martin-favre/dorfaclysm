#pragma once
#include <string>

class Uuid {
 public:
  static Uuid generateNew();
  bool operator==(const Uuid& other);
 private:
  Uuid(const std::string& rawUuid);
  const std::string mUuid;
};
