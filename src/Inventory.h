#pragma once

#include <cstddef>
#include <list>
#include <memory>

#include "Component.h"
#include "ItemType.h"
#include "Serializer.h"
#include "SpriteSheetInfo.h"
class Item;
class GameObject;

class Inventory : public Component {
 public:
  Inventory(GameObject& owner);
  Inventory(GameObject& owner, const SerializedObj& serObj);
  void addItem(Item&& item);
  Item getItem(ItemType type);
  bool containsItem(ItemType type) const;
  size_t count() const;
  SerializedObj serialize() const override;
  static std::string getTypeString() { return "Inventory"; }

 private:
  std::list<Item> mItems;
};
