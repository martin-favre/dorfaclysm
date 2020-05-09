#include "Component.h"
const std::string Component::SerializeString_Type{"type"};

bool& Component::enabled() { return mEnabled; }

SerializedObj Component::serialize() const {
  SerializedObj out;
  out["type"] = "Component";
  out["enabled"] = mEnabled;
  return out;
}

void Component::unserialize(const SerializedObj& serObj) {
  serObj.at("enabled").get_to(mEnabled);
}
