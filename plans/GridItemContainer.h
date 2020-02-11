#pragma once
#include <list>
#include <memory>

#include "Component.h"
#include "Item.h"
#include "VisibleList.h"

class GridItemContainer : public Component, public VisibleList<Item> {
 public:
  GridItemContainer(GameObject& gObj);
 private:
};