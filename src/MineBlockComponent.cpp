#include "MineBlockComponent.h"

#include "Block.h"
#include "Component.h"
#include "GameObject.h"
#include "GridMap.h"
#include "MiningRequestPool.h"
MineBlockComponent::MineBlockComponent(GameObject& gObj) : Component(gObj) {}
MineBlockComponent::MineBlockComponent(GameObject& gObj, const SerializedObj& serObj)
    : Component(gObj, serObj[SerializeString_Parent]) {}

SerializedObj MineBlockComponent::serialize() const {
  SerializedObj out = createSerializedObj<MineBlockComponent>();
  out[SerializeString_Parent] = Component::serialize();
  return out;
}

void MineBlockComponent::onJobComplete() { owner().destroy(); }

void MineBlockComponent::setup() {
  const Block& block =
      GridMap::getActiveMap().getBlockAt(owner().getPosition());
  if (block.supportsJob(requestTypeMining)) {
    std::function<void()> callback = [this]() { this->onJobComplete(); };
    auto ptr = std::make_unique<MiningRequest>(
        block.getIdentifier(), owner().getPosition(), owner().getIdentifier());
    MiningRequestPool::getInstance().addRequest(std::move(ptr));
  } else {
    onJobComplete();
  }
}
