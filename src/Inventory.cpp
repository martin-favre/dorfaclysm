#include "Inventory.h"

#include <memory>

#include "Component.h"
#include "Item.h"
#include "ItemType.h"
#include "Serializer.h"

void Inventory::addItem(Item&& item) {
  auto iter{mItems.find(item.getItemType())};
  if (iter == mItems.end()) {
    mItems.emplace(item.getItemType(), std::move(item));
  } else {
    iter->second.addItems(1);
  }
  if(mOnItemAdded) mOnItemAdded();
}

Item Inventory::getItem(ItemType type) {
  auto iter{mItems.find(type)};
  if (iter != mItems.end()) {
    Item& myItem = iter->second;
    Item out{myItem.getItemType()};
    myItem.addItems(-1);
    if (myItem.isEmpty()) mItems.erase(iter);
    return out;
  }
  return Item(ItemType_Invalid);
}

bool Inventory::containsItem(ItemType type) const { return mItems.count(type); }

size_t Inventory::countItemTypes() const { return mItems.size(); }

size_t Inventory::countNumberOfItem(ItemType type) const {
  auto iter = mItems.find(type);
  if (iter != mItems.end()) {
    return iter->second.getCount();
  } else {
    return 0;
  }
}

Inventory::Inventory(GameObject& owner) : Component(owner) {}
Inventory::Inventory(GameObject& owner, const SerializedObj& serObj)
    : Component(owner, serObj[SerializeString_Parent]) {
  std::vector<SerializedObj> items = serObj["items"];
  for (const auto& item : items) {
    addItem(Item(item));
  }
}

SerializedObj Inventory::serialize() const {
  SerializedObj out = createSerializedObj<Inventory>();
  out[SerializeString_Parent] = Component::serialize();
  std::vector<SerializedObj> items;
  for (const auto& item : mItems) {
    items.emplace_back(item.second);
  }
  out["items"] = items;
  return out;
}

void Inventory::registerOnItemAddedCallback(std::function<void()> onItemChanged) {
  mOnItemAdded = onItemChanged;
}

// void to_json(SerializedObj& out, const Inventory& inv) {
//   out = createSerializedObj<Inventory>()
//   std::vector<SerializedObj> items;
//   for (const auto& item : inv.mItems) {
//     items.emplace_back(*item);
//   }
//   out["items"] = items;
// }

// void from_json(const SerializedObj& in, Inventory& inv) {
//   std::vector<SerializedObj> items = in["items"];
//   for (const auto& item : items) {
//     inv.mItems.emplace_back(std::make_unique<Item>(item));
//   }
// }