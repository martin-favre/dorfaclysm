#include "RequestPoolComponent.h"

#include "BlockBuildingRequestPool.h"
#include "Component.h"
#include "MiningRequestPool.h"
#include "MoveItemRequestPool.h"

RequestPoolComponent::RequestPoolComponent(GameObject& owner)
    : Component(owner) {}
RequestPoolComponent::RequestPoolComponent(GameObject& owner,
                                           const SerializedObj& serObj)
    : Component(owner, serObj[SerializeString_Parent]) {}

void RequestPoolComponent::teardown() {
  MiningRequestPool::getInstance().clearRequests();
  MoveItemRequestPool::getInstance().clearRequests();
  BlockBuildingRequestPool::getInstance().clearRequests();
}

SerializedObj RequestPoolComponent::serialize() const {
  SerializedObj out = createSerializedObj<RequestPoolComponent>();
  out[SerializeString_Parent] = Component::serialize();
  return out;
}

std::string RequestPoolComponent::getTypeString() {
  return "RequestPoolComponent";
}