#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "Component.h"
#include "Serializer.h"
#include "Uuid.h"
#include "Vector2D.h"
#include "Vector3DInt.h"

class GameObject {
 public:
  GameObject(const SerializedObj& serObj);
  GameObject();
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

  template <class componentType>
  bool hasComponent();

  /*-------------------------------------------------------
  Larger renderDepth is closer to the camera
  Can only be changed in scene creation
  Change will otherwise not have an effect
  ---------------------------------------------------------*/
  int getRenderDepth() const;
  void setRenderDepth(int depth);
  Vector3DInt getPosition() const;
  void setPosition(const Vector3DInt& pos);
  Vector2D getScale() const;
  void setScale(const Vector2D& newScale);
  double getRotation() const;
  const Uuid& getIdentifier() const;

  /*-------------------------------------------------------
  Orders the removal of the GameObject. It will be removed
  at the end of the current tick.
  All pointers to this GameObject
  will then be invalid.
  ---------------------------------------------------------*/
  void destroy();
  void setName(const std::string&);
  const std::string& name() const;

  SerializedObj serialize() const;
  
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

 private:
  void unserializeComponents(const std::vector<SerializedObj>& components);

  Vector3DInt mPosition;
  int mRenderDepth{0};
  Vector2D mScale{1, 1};
  double mRotation{0};
  bool mEnabled{true};
  std::string mName{"NoName"};
  const Uuid mIdentifier;

  std::vector<std::unique_ptr<Component>> mComponents;

  std::mutex mMutex;
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

template <class componentType>
bool GameObject::hasComponent() {
  return getComponent<componentType>() != nullptr;
}
