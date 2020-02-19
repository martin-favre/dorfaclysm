#include "MineBlockComponent.h"

#include "Block.h"
#include "GameObject.h"
#include "GridMap.h"
#include "MiningRequestPool.h"
MineBlockComponent::MineBlockComponent(GameObject& gObj) : Component(gObj) {}

void MineBlockComponent::onJobComplete() { owner().destroy(); }

void MineBlockComponent::setup() {
  std::weak_ptr<Block> block =
      GridMap::getActiveMap().getBlockPtrAt(owner().getPosition());
  if (block.lock()->supportsJob(requestTypeMining)) {
    std::function<void()> callback = [this]() { this->onJobComplete(); };
    auto ptr =
        std::make_unique<MiningRequest>(block, owner().getPosition(), callback);
    MiningRequestPool::getInstance().addRequest(std::move(ptr));
  } else {
    onJobComplete();
  }
}
