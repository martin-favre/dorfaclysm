#include "GameObject.h"

#include "Engine.h"
GameObject::GameObject(GAMEOBJECT_ID id) : mId(id) {}

GameObject::~GameObject() { mComponents.clear(); }

bool& GameObject::enabled() { return mEnabled; }

void GameObject::setup() {
  for (auto& component : mComponents) {
    component->setup();
  }
}

void GameObject::teardown() {
  for (auto& component : mComponents) {
    component->teardown();
  }
}

void GameObject::updateComponents() {
  if (!mEnabled) return;
  for (auto& component : mComponents) {
    component->update();
  }
}

void GameObject::render() {
  if (!mEnabled) return;
  std::scoped_lock lock(mMutex);
  for (auto& component : mComponents) {
    component->render();
  }
}

void GameObject::setPosition(const Vector3DInt& pos) { 
  std::scoped_lock lock(mMutex);
  mPosition = pos; 
  }

int GameObject::getRenderDepth() const { return mRenderDepth; }
void GameObject::setRenderDepth(int depth) { 
  std::scoped_lock lock(mMutex);
  mRenderDepth = depth; }

Vector3DInt GameObject::getPosition() const { return mPosition; }

Vector2D GameObject::getScale() const { return mScale; }
void GameObject::setScale(const Vector2D& newScale) { 
  std::scoped_lock lock(mMutex);
  mScale = newScale; }
double GameObject::getRotation() const { return mRotation; }

void GameObject::destroy() { Engine::removeGameObject(this); }

GAMEOBJECT_ID
GameObject::id() const { return mId; }
std::string& GameObject::name() { return mName; }
const std::string& GameObject::name() const { return mName; }
