#pragma once
#include <list>
#include <memory>

#include "Component.h"
#include "GridActor.h"
#include "Inventory.h"
#include "Item.h"
#include "Serializer.h"
class ItemContainer : public Component {
 public:
  ItemContainer(GameObject& gObj);
  ItemContainer(GameObject& gObj, const SerializedObj& serObj);
  void setup() override;
  SerializedObj serialize() const override;
  void teardown() override;
  void addItem(Item&& item);
  Item getItem(ItemType type);
  bool containsItemType(ItemType type) const;
  static std::string getTypeString();

 private:
  Inventory* mInventory{};
};