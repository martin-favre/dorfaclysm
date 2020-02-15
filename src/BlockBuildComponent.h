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
      if (actor->getType() == GridActor::item) {
        ItemContainer* cont = actor->owner().getComponent<ItemContainer>();
        ASSERT(cont, "ItemObject without Itemcontainer, not ok");
        std::unique_ptr<Item> item = cont->getItem(mNeededItem);
        if (item) {
          ASSERT(item->isPlaceable(), "This should be a placeable item");
          IPlaceableItem* placeableItem =
              dynamic_cast<IPlaceableItem*>(item.get());
          ASSERT(placeableItem, "Says it's placeable, so should be placeable");
          mGridMap.setBlockAt(owner().getPosition(), placeableItem->getBlock());
          Engine::removeGameObject(&owner());
          mGridMap.getBlockAt(owner().getPosition()).setExplored();
        }
      }
    }
  }

 private:
  GridActor* mGridActor{};
  GridMap& mGridMap;
  ItemType mNeededItem;
};