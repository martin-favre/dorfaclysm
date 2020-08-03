#pragma once

#include "Component.h"
#include "GameObject.h"
#include "GridActor.h"
#include "GridMap.h"
#include "Inventory.h"
#include "ItemType.h"
#include "Logging.h"
#include "MoveItemRequestPool.h"
#include "Uuid.h"
#include "Vector3DInt.h"
class BuildThingComponent : public Component {
 public:
  BuildThingComponent(GameObject& gObj,
                      std::list<std::pair<ItemType, uint>>&& requiredItems,
                      std::function<void(const Vector3DInt&)> createThing)
      : Component(gObj),
        mRequiredItems(std::move(requiredItems)),
        mCreationFunc(createThing) {}

  void setup() {
    mGainedItems = owner().getComponent<Inventory>();
    if (mGainedItems) {
      mGainedItems->registerOnItemAddedCallback([this]() { this->onItemChanged(); });
    } else {
      LOGL("Could not find inventory on BuildThingComponent", Logging::error);
      owner().destroy();
      return;
    }

    MoveItemRequestPool& pool = MoveItemRequestPool::getInstance();
    for (const auto& itemType : mRequiredItems) {
      for (uint i = 0; i < itemType.second; ++i) {
        pool.addRequest(std::make_unique<MoveItemRequest>(
            itemType.first, owner().getPosition(),
            std::make_unique<Uuid>(owner().getIdentifier())));
      }
    }
  }

 private:
  void onItemChanged() {
    for (const auto& itemType : mRequiredItems) {
      if (mGainedItems->countNumberOfItem(itemType.first) < itemType.second) {
        return;
      }
    }
    mCreationFunc(owner().getPosition());
    owner().destroy();
  }
  std::list<std::pair<ItemType, uint>> mRequiredItems;
  std::function<void(const Vector3DInt&)> mCreationFunc;
  Inventory* mGainedItems{};
};