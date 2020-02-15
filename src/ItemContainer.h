#pragma once
#include <list>
#include <memory>

#include "Component.h"
#include "Item.h"
#include "GridActor.h"

class ItemContainer : public Component {
 public:
  ItemContainer(GameObject& gObj);
  void teardown()override;
  void addItem(std::unique_ptr<Item> && item);
  std::unique_ptr<Item> getItem(ItemType type);
  bool containsItemType(ItemType type) const;
 private:
 std::vector<std::unique_ptr<Item>> mItems;
};