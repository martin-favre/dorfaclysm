#pragma once
typedef unsigned long GAMEOBJECT_ID;

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "Vector2D.h"
#include "Vector2DInt.h"

class GameObject {
 public:
  GameObject(GAMEOBJECT_ID id);
  virtual ~GameObject();
  GameObject(const GameObject&) = delete;
  GameObject& operator=(const GameObject&) = delete;

  /*-------------------------------------------------------
  Get/Set if the gameobject is enabled.
  ---------------------------------------------------------
  @return reference to if the variable is enabled.
  ---------------------------------------------------------*/
  bool& enabled();

  template <typename T, class... Args>
  T& addComponent(Args&&... args) {
    auto newComp = std::make_unique<T>(*this, std::forward<Args>(args)...);
    auto newCompPtr = newComp.get();
    mComponents.emplace_back(std::move(newComp));
    return *newCompPtr;
  }

  /*-------------------------------------------------------
  Gets pointer to component of type.
  Returns nullptr if GameObject does not have a component.
  If GameObject has two of that component one of them will
  be returned. No Guarantee which one.
  ---------------------------------------------------------
  @return pointer to component. nullptr if no component found.
  ---------------------------------------------------------*/
  template <class componentType>
  componentType* getComponent();

  /*-------------------------------------------------------
  Larger renderDepth is closer to the camera
  Can only be changed in scene creation
  Change will otherwise not have an effect
  ---------------------------------------------------------*/
  int getRenderDepth() const;
  void setRenderDepth(int depth);
  const Vector2DInt& getPosition() const;
  void setPosition(const Vector2DInt& pos);
  const Vector2D& getScale() const;
  void setScale(const Vector2D& newScale);
  double getRotation() const;

  /*-------------------------------------------------------
  Orders the removal of the GameObject. It will be removed
  at the end of the current tick.
  All pointers to this GameObject
  will then be invalid.
  ---------------------------------------------------------*/
  void destroy();
  GAMEOBJECT_ID id() const;
  std::string& name();

 private:
  friend class Engine;
  /*-------------------------------------------------------
  Runs all components' setups.
  ---------------------------------------------------------*/
  void setup();

  /*-------------------------------------------------------
  Runs all components' teardowns.
  ---------------------------------------------------------*/
  void teardown();

  /*-------------------------------------------------------
  Runs all components' renders.
  ---------------------------------------------------------*/
  virtual void render();

  /*-------------------------------------------------------
  Runs all components' update.
  ---------------------------------------------------------*/
  void updateComponents();

  Vector2DInt mPosition;
  int mRenderDepth{0};
  Vector2D mScale{1, 1};
  double mRotation{0};
  bool mEnabled{true};
  std::string mName{"NoName"};
  const GAMEOBJECT_ID mId{0};
  std::vector<std::unique_ptr<Component>> mComponents;
};

template <class componentType>
componentType* GameObject::getComponent() {
  for (const auto& item : mComponents) {
    auto raw = item.get();
    auto casted = dynamic_cast<componentType*>(raw);
    if (casted) {
      return casted;
    }
  }
  return nullptr;
}
