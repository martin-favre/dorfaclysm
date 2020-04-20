#include "GameObject.h"

#include "Engine.h"
#include "StringToComponent.h"

GameObject::GameObject(const SerializedObj& serObj) { unserialize(serObj); }

GameObject::GameObject() {}

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
  mRenderDepth = depth;
}

Vector3DInt GameObject::getPosition() const { return mPosition; }

Vector2D GameObject::getScale() const { return mScale; }
void GameObject::setScale(const Vector2D& newScale) {
  std::scoped_lock lock(mMutex);
  mScale = newScale;
}
double GameObject::getRotation() const { return mRotation; }

void GameObject::destroy() { Engine::removeGameObject(this); }

std::string& GameObject::name() { return mName; }
const std::string& GameObject::name() const { return mName; }

SerializedObj GameObject::serialize() const {
  SerializedObj j;
  j["position"] = mPosition;
  j["renderDepth"] = mRenderDepth;
  j["scale"] = mScale;
  j["rotation"] = mRotation;
  j["enabled"] = mEnabled;
  j["name"] = mName;
  std::vector<SerializedObj> components;
  for (const auto& c : mComponents) {
    components.emplace_back(c->serialize());
  }
  j["components"] = components;
  return j;
}

void GameObject::unserialize(const SerializedObj& j) {
  j.at("position").get_to(mPosition);
  j.at("renderDepth").get_to(mRenderDepth);
  j.at("scale").get_to(mScale);
  j.at("rotation").get_to(mRotation);
  j.at("enabled").get_to(mEnabled);
  j.at("name").get_to(mName);
  std::vector<SerializedObj> components;
  j.at("components").get_to(components);
  for (const auto& c : components) {
    std::string type = c.at("type");
    if(StringToComponent::unserializeComponentMap.count(type)){
      StringToComponent::unserializeComponentMap.at(type)(*this, c);
    }
  }
}