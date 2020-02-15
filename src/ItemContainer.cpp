#include "ItemContainer.h"

#include "Engine.h"
#include "GameObject.h"
#include "ItemPool.h"

ItemContainer::ItemContainer(GameObject& gObj) : Component(gObj) {
  ItemPool::registerItem(this);
}
void ItemContainer::teardown() { ItemPool::unRegisterItem(this); }
void ItemContainer::addItem(std::unique_ptr<Item>&& item) {
  mItems.emplace_back(std::move(item));
}

std::unique_ptr<Item> ItemContainer::getItem(ItemType type) {
  for (auto it = mItems.begin(); it != mItems.end(); ++it) {
    if ((*it)->isType(type)) {
      std::unique_ptr<Item> item = std::move(*it);
      mItems.erase(it);
      if (mItems.size() == 0) {
        Engine::removeGameObject(&owner());
      }
      return item;
    }
  }
  return nullptr;
}

bool ItemContainer::containsItemType(ItemType type) const {
  for (const auto& item : mItems) {
    if (item->isType(type)) return true;
  }
  return false;
}
