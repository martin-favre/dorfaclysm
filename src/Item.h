#pragma once
#include <string>
#include <typeindex>

#include "Sprite.h"

class ItemType {
 public:
  ItemType(const std::type_index& type) : mType(type) {}
  inline bool operator==(const ItemType& other) const {
    return mType == other.mType;
  }

 private:
  const std::type_index mType;
};

template <typename T>
inline ItemType generateItemType() {
  return ItemType(typeid(T));
}

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