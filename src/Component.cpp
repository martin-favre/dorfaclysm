#include "Component.h"
const std::string Component::SerializeString_Type{"component_type"};
const std::string Component::SerializeString_Parent{"component_parent"};

Component::Component(GameObject& user) : mOwner(user) {
  LOG("Adding component " << this);
}
Component::Component(GameObject& user, const SerializedObj& serObj)
    : mEnabled(serObj.at("enabled")), mOwner(user) {
  LOG("Loading component " << this);
}

bool& Component::enabled() { return mEnabled; }

SerializedObj Component::serialize() const {
  SerializedObj out;
  // explicitly don't create getTypeString in Component
  // so children must create it;
  out[Component::SerializeString_Type] = "Component";
  out["enabled"] = mEnabled;
  return out;
}