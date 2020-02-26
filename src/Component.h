#pragma once
class GameObject;
#include "Logging.h"

class Component {
 public:
  Component(GameObject& user) : mOwner(user) 
  {
    LOG("Adding component " << this);
  }
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;
  virtual ~Component() {
    LOG("Deleting component " << this);
  }
  virtual void setup() {}
  virtual void update() {}
  virtual void render() {}
  virtual void teardown() {}
  bool& enabled();
  GameObject& owner() { return mOwner; }
  const GameObject& owner() const { return mOwner; }

 private:
  bool mEnabled = true;

  GameObject& mOwner;
};