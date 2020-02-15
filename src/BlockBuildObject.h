#pragma once
#include "GameObject.h"
class ItemType;
class BlockBuildObject : public GameObject {
 public:
  BlockBuildObject(GAMEOBJECT_ID id, const ItemType& item);
};