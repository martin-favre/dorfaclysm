#pragma once
#include <map>
#include <list>
#include "Item.h"

class Comparer;
class Vector3DInt;
class ItemContainer;
class ItemPool
{
  public:

  static bool whereIsClosestItem(const Vector3DInt& pos, Vector3DInt& out, ItemType type);
  static void registerItem(ItemContainer* item);
  static void unRegisterItem(ItemContainer* item);

  private:
  static std::list<ItemContainer*> mItems;
};