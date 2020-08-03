#pragma once
#include <string>

#include "Serializer.h"

class Uuid {
 public:
  Uuid(const Uuid&) = default;
  Uuid(const SerializedObj& rawUuid);
  static Uuid generateNew();
  bool operator==(const Uuid& other) const;
  friend void to_json(SerializedObj& out, const Uuid& vec);

 private:
  Uuid(const std::string& rawUuid);
  std::string mUuid;
};

void to_json(SerializedObj& out, const Uuid& vec);