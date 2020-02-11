#pragma once
#include <list>
#include <memory>

#include "Component.h"
#include "Item.h"

class GridItemContainer : public Component {
 public:
  GridItemContainer(GameObject& gObj);
  void addItem(std::unique_ptr<Item>&& item);
  const std::list<std::unique_ptr<Item>>& getItems();
  std::unique_ptr<Item> claimItem(
      std::list<std::unique_ptr<Item>>::const_iterator which);

 private:
  std::list<std::unique_ptr<Item>> mItems;
};