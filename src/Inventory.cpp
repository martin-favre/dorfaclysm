#include "Inventory.h"

#include <memory>

#include "Component.h"
#include "Item.h"
#include "ItemType.h"
#include "Serializer.h"

void Inventory::addItem(Item&& item) { mItems.emplace_back(std::move(item)); }

Item Inventory::getItem(ItemType type) {
  for (auto it = mItems.begin(); it != mItems.end(); ++it) {
    if ((*it).getItemType() == type) {
      Item item = std::move(*it);
      mItems.erase(it);
      return item;
    }
  }
  return Item(ItemType_Invalid);
}

bool Inventory::containsItem(ItemType type) const {
  for (const auto& item : mItems) {
    if (item.getItemType() == type) return true;
  }
  return false;
}

size_t Inventory::count() const { return mItems.size(); }

Inventory::Inventory(GameObject& owner) : Component(owner) {}
Inventory::Inventory(GameObject& owner, const SerializedObj& serObj)
    : Component(owner, serObj[SerializeString_Parent]) {
  std::vector<SerializedObj> items = serObj["items"];
  for(const auto& item : items){
    mItems.emplace_back(item);
  }
}

SerializedObj Inventory::serialize() const {
  SerializedObj out = createSerializedObj<Inventory>();
  out[SerializeString_Parent] = Component::serialize();
  std::vector<SerializedObj> items;
  for (const auto& item : mItems) {
    items.emplace_back(item);
  }
  out["items"] = items;
  return out;
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