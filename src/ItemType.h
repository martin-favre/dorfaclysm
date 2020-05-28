#pragma once
#include <memory>
enum ItemType {
  ItemType_RockBlockItem,
};
class Item;
std::unique_ptr<Item> generateItem(ItemType type);