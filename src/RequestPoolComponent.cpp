#include "RequestPoolComponent.h"

#include "BlockBuildingRequestPool.h"
#include "MiningRequestPool.h"
#include "MoveItemRequestPool.h"

RequestPoolComponent::RequestPoolComponent(GameObject& owner)
    : Component(owner) {}
RequestPoolComponent::RequestPoolComponent(GameObject& owner,
                                           const SerializedObj& serObj)
    : Component(owner) {
  serObj.at("miningRequestPool").get_to(MiningRequestPool::getInstance());
  serObj.at("moveItemRequestPool").get_to(MoveItemRequestPool::getInstance());
  serObj.at("buildBlockRequestPool")
      .get_to(BlockBuildingRequestPool::getInstance());
}

SerializedObj RequestPoolComponent::serialize() const {
  SerializedObj out = createSerializedObj<RequestPoolComponent>();
  out["miningRequestPool"] = MiningRequestPool::getInstance();
  out["moveItemRequestPool"] = MoveItemRequestPool::getInstance();
  out["buildBlockRequestPool"] = BlockBuildingRequestPool::getInstance();
  return out;
}

std::string RequestPoolComponent::getTypeString() {
  return "RequestPoolComponent";
}