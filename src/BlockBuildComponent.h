#pragma once
#include "Component.h"
#include "Engine.h"
#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "IPlaceableItem.h"
#include "Item.h"
#include "ItemContainer.h"
#include "MoveItemRequestPool.h"

class BlockBuildComponent : public Component {
 public:
  BlockBuildComponent(GameObject& gObj, const ItemType& item)
      : Component(gObj), mGridMap(GridMap::getActiveMap()), mNeededItem(item) {}

  void setup() override {
    mGridActor = owner().getComponent<GridActor>();
    MoveItemRequestPool::getInstance().addRequest(
        std::make_unique<MoveItemRequest>(mNeededItem, owner().getPosition()));
  }

  void update() override {
    const auto& actors = mGridMap.getGridActorsAt(owner().getPosition());
    for (const auto& actor : actors) {
      ItemContainer* cont = actor->owner().getComponent<ItemContainer>();
      if (!cont) continue;

      Item item = cont->getItem(mNeededItem);
      if (item.isValid() && item.isPlaceable()) {
        mGridMap.setBlockAt(owner().getPosition(), item.getBlockType());
        Engine::removeGameObject(&owner());
        mGridMap.getBlockAt(owner().getPosition()).setExplored();
      }
    }
  }

 private:
  GridActor* mGridActor{};
  GridMap& mGridMap;
  ItemType mNeededItem;
};