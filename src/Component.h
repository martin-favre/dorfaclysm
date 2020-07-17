#pragma once
class GameObject;
#include "Logging.h"
#include "Serializer.h"
class Component {
 public:
  Component(GameObject& user);
  Component(GameObject& user, const SerializedObj& serObj);
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;
  virtual ~Component() { LOG("Deleting component " << this); }
  virtual void setup() {}
  virtual void update() {}
  virtual void render() {}
  virtual void teardown() {}
  virtual SerializedObj serialize() const;
  bool& enabled();
  GameObject& owner() { return mOwner; }
  const GameObject& owner() const { return mOwner; }
  static const std::string SerializeString_Type;
  static const std::string SerializeString_Parent;

 protected:
 private:
  bool mEnabled = true;
  GameObject& mOwner;
};

template <typename T>
SerializedObj createSerializedObj() {
  SerializedObj out;
  out[Component::SerializeString_Type] = T::getTypeString();
  return out;
}