#pragma once
#include <string>
#include <typeindex>
#include "ItemType.h"
#include "Sprite.h"

class Item {
 public:
  virtual ~Item() = default;
  virtual const std::string& getName() const = 0;
  virtual const std::string& getDescription() const = 0;
  virtual const Sprite& getSprite() const = 0;
  virtual uint getWorth() const = 0;
  virtual const ItemType& getItemType() const = 0;

  // If true, then it also inherits from IPlaceableItem
  virtual bool isPlaceable() const = 0;
};