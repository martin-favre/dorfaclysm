#pragma once
class GameObject;

class Component {
 public:
  Component(GameObject& user) : mOwner(user) {}
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;
  virtual void setup() {}
  virtual void update() {}
  virtual void render() {}
  virtual void teardown() {}
  virtual ~Component() {}
  bool& enabled();
  GameObject& owner() { return mOwner; }
  const GameObject& owner() const { return mOwner; }

 private:
  bool mEnabled = true;

  GameObject& mOwner;
};