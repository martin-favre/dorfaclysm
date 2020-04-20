#pragma once
#include "GameObject.h"
class ItemType;
class BlockBuildObject : public GameObject {
 public:
  BlockBuildObject(const ItemType& item);
};