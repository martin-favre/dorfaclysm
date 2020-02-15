#include "ItemPool.h"

#include <algorithm>

#include "GameObject.h"
#include "ItemContainer.h"
#include "Vector3DInt.h"
std::list<ItemContainer*> ItemPool::mItems;

bool ItemPool::whereIsClosestItem(const Vector3DInt& pos, Vector3DInt& out,
                                  ItemType type) {
  Vector3DInt closest;
  double minDistance = DBL_MAX;
  for (auto cont : mItems) {
    if (cont->containsItemType(type)) {
      Vector3DInt diff = cont->owner().getPosition() - pos;
      double magn = diff.squared_magnitude();
      if (magn < minDistance) {
        minDistance = magn;
        closest = cont->owner().getPosition();
      }
    }
  }
  if (minDistance != DBL_MAX) {
    out = closest;
    return true;
  }
  return false;
}
void ItemPool::registerItem(ItemContainer* item) {
  ASSERT(item, "Received null item ptr");
  mItems.emplace_back(item);
}
void ItemPool::unRegisterItem(ItemContainer* item) {
  ASSERT(item, "Received null item ptr");
  auto it = std::find(mItems.begin(), mItems.end(), item);
  ASSERT(it != mItems.end(), "Could not unregister, item was not in list");
  mItems.erase(it);
}
