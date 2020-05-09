#pragma once
class GameObject;
#include "Logging.h"
#include "Serializer.h"
class Component {
 public:
  Component(GameObject& user) : mOwner(user) {
    LOG("Adding component " << this);
  }
  Component(GameObject& user, const SerializedObj& serObj) : mOwner(user) {
    LOG("Loading component " << this);
    unserialize(serObj);
  }
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

  protected:

 private:
  virtual void unserialize(const SerializedObj& serObj);
  bool mEnabled = true;

  GameObject& mOwner;
};