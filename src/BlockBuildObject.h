#pragma once
#include "GameObject.h"
#include "ItemType.h"
class BlockBuildObject : public GameObject {
 public:
  BlockBuildObject(const ItemType& item);
};