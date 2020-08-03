#pragma once
#include "Component.h"
#include "Engine.h"
#include "GameObject.h"
#include "GridMap.h"
#include "IPlaceableItem.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemContainer.h"
#include "MoveItemRequestPool.h"

class BlockBuildComponent : public Component {
 public:
  BlockBuildComponent(GameObject& gObj, const ItemType& item)
      : Component(gObj), mGridMap(GridMap::getActiveMap()), mNeededItem(item) {}

  void setup() override {
    mInventory = owner().getComponent<Inventory>();

    if (!mInventory) {
      LOGL("No Inventory on BlockBuildComponent", Logging::error);
      owner().destroy();
      return;
    } else {
      mInventory->registerOnItemAddedCallback([this](){this->onItemChanged();});
    }

    MoveItemRequestPool::getInstance().addRequest(
        std::make_unique<MoveItemRequest>(
            mNeededItem, owner().getPosition(),
            std::make_unique<Uuid>(owner().getIdentifier())));
  }

 private:
  void onItemChanged() {
    if (mInventory->containsItem(mNeededItem)) {
      mGridMap.setBlockAt(owner().getPosition(),
                          mInventory->getItem(mNeededItem).getBlockType());
      mGridMap.getBlockAt(owner().getPosition()).setExplored();
      owner().destroy();
    }
    owner().destroy();
  }

  void buildBlock() {}
  Inventory* mInventory{};
  GridMap& mGridMap;
  ItemType mNeededItem;
};