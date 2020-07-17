#pragma once
#include "Component.h"
#include "Serializer.h"

// Has the purpose to serialize and unserialize all requestPools.
// One and only one should exist in each scene.
class RequestPoolComponent : public Component {
 public:
  RequestPoolComponent(GameObject& owner);
  RequestPoolComponent(GameObject& owner, const SerializedObj& serObj);
  static std::string getTypeString();
  SerializedObj serialize() const override;
};