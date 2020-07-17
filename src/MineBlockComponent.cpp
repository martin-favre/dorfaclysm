#include "MineBlockComponent.h"

#include "Block.h"
#include "GameObject.h"
#include "GridMap.h"
#include "MiningRequestPool.h"
MineBlockComponent::MineBlockComponent(GameObject& gObj) : Component(gObj) {}

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
