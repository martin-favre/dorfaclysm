#include "ItemContainer.h"

#include "Component.h"
#include "Engine.h"
#include "GameObject.h"
#include "ItemPool.h"
#include "ItemType.h"
#include "Serializer.h"
#include <memory>

ItemContainer::ItemContainer(GameObject& gObj) : Component(gObj) {
  ItemPool::registerItem(this);
}

ItemContainer::ItemContainer(GameObject& gObj, const SerializedObj& serObj) : Component(gObj, serObj["parent"]){
  std::vector<SerializedObj> items = serObj["items"];
  for(const auto& item : items){
    mItems.emplace_back(generateItem(item));
  }
  ItemPool::registerItem(this);
}
  SerializedObj ItemContainer::serialize() const{
    SerializedObj out = createSerializedObj<ItemContainer>();
    out["parent"] = Component::serialize();
    std::vector<SerializedObj> items;
    for(const auto& item : mItems){
      items.emplace_back(item->getItemType());
    }
    out["items"] = items;
    return out;
  }

std::string ItemContainer::getTypeString(){
  return "ItemContainer";
}

void ItemContainer::teardown() { ItemPool::unRegisterItem(this); }
void ItemContainer::addItem(std::unique_ptr<Item>&& item) {
  mItems.emplace_back(std::move(item));
}

std::unique_ptr<Item> ItemContainer::getItem(ItemType type) {
  for (auto it = mItems.begin(); it != mItems.end(); ++it) {
    if ((*it)->getItemType() == type) {
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
    if (item->getItemType() == type) return true;
  }
  return false;
}
