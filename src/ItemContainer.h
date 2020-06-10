#pragma once
#include <list>
#include <memory>

#include "Component.h"
#include "Item.h"
#include "GridActor.h"
#include "Serializer.h"

class ItemContainer : public Component {
 public:
  ItemContainer(GameObject& gObj);
  ItemContainer(GameObject& gObj, const SerializedObj& serObj);
  SerializedObj serialize() const override;
  void teardown()override;
  void addItem(std::unique_ptr<Item> && item);
  std::unique_ptr<Item> getItem(ItemType type);
  bool containsItemType(ItemType type) const;
  static std::string getTypeString();
 private:
 std::vector<std::unique_ptr<Item>> mItems;
};