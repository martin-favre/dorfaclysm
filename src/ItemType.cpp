#include "ItemType.h"
#include "Helpers.h"
#include "RockBlockItem.h"

std::unique_ptr<Item> generateItem(ItemType type) {
  switch (type) {
    case ItemType_RockBlockItem:
      return std::make_unique<RockBlockItem>();
      break;

    default:
      ASSERT(false, "unknown itemtype");
      break;
  }
}