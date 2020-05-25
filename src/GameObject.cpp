#include "GameObject.h"

#include "Engine.h"
#include "StringToComponent.h"

GameObject::GameObject(const SerializedObj& serObj)
    : mPosition(serObj.at("position")),
      mRenderDepth(serObj.at("renderDepth")),
      mScale(serObj.at("scale")),
      mRotation(serObj.at("rotation")),
      mEnabled(serObj.at("enabled")),
      mIdentifier(serObj.at("identifier")) {
  unserializeComponents(serObj.at("components"));
}

GameObject::GameObject() : mIdentifier(Uuid::generateNew()) {}

GameObject::~GameObject() { mComponents.clear(); }

SerializedObj GameObject::serialize() const {
  SerializedObj out;
  out["position"] = mPosition;
  out["renderDepth"] = mRenderDepth;
  out["scale"] = mScale;
  out["rotation"] = mRotation;
  out["enabled"] = mEnabled;
  out["name"] = mName;
  out["identifier"] = mIdentifier;
  std::vector<SerializedObj> components;
  for (const auto& c : mComponents) {
    components.emplace_back(c->serialize());
  }
  out["components"] = components;
  return out;
}

void GameObject::unserializeComponents(
    const std::vector<SerializedObj>& components) {
  for (const auto& c : components) {
    std::string type = c.at(Component::SerializeString_Type);
    if (StringToComponent::unserializeComponentMap.count(type)) {
      StringToComponent::unserializeComponentMap.at(type)(*this, c);
    } else {
      LOGL("Cannot unserialize type, unknown type", Logging::warning);
    }
  }
}

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

const Uuid& GameObject::getIdentifier() const { return mIdentifier; }

void GameObject::destroy() { Engine::removeGameObject(this); }

void GameObject::setName(const std::string& name) { mName = name; }

const std::string& GameObject::name() const { return mName; }
