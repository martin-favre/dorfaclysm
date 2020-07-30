#include "ItemContainer.h"

#include <memory>
#include <utility>

#include "Component.h"
#include "Engine.h"
#include "GameObject.h"
#include "ItemPool.h"
#include "ItemType.h"
#include "Logging.h"
#include "Serializer.h"

ItemContainer::ItemContainer(GameObject& gObj) : Component(gObj) {
  ItemPool::registerItem(this);
}

ItemContainer::ItemContainer(GameObject& gObj, const SerializedObj& serObj)
    : Component(gObj, serObj["parent"]) {
  ItemPool::registerItem(this);
}

SerializedObj ItemContainer::serialize() const {
  SerializedObj out = createSerializedObj<ItemContainer>();
  out["parent"] = Component::serialize();
  return out;
}

void ItemContainer::setup() {
  mInventory = owner().getComponent<Inventory>();
  if (!mInventory) {
    owner().destroy();
    LOGL("Itemcontainer without inventory", Logging::error);
  }
}

std::string ItemContainer::getTypeString() { return "ItemContainer"; }

void ItemContainer::teardown() { ItemPool::unRegisterItem(this); }
void ItemContainer::addItem(Item&& item) {
  if (mInventory) {
    mInventory->addItem(std::move(item));
  }
}

Item ItemContainer::getItem(ItemType type) {
  if (!mInventory) return Item::invalidItem();
  Item item = mInventory->getItem(type);
  if (mInventory->count() == 0) owner().destroy();
  return item;
}

bool ItemContainer::containsItemType(ItemType type) const {
  return mInventory->containsItem(type);
}
