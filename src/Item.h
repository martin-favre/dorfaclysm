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
inline ItemType getItemType() {
  return ItemType(typeid(T));
}

class Item {
 public:
  virtual ~Item() = default;
  const std::string& getName() const;
  const std::string& getDescription() const;
  const Sprite& getSprite() const;
  uint getWorth() const;
  inline bool isType(const ItemType& type) const { return mType == type; }
  inline bool sameItemType(const Item& other) const {
    return other.mType == mType;
  }

  // If true, then it also inherits from IPlaceableItem
  virtual bool isPlaceable() const = 0;

 protected:
  Item(const Sprite& sprite, const std::string& name, const std::string& desc,
       uint worth, const ItemType& type);

 private:
  const Sprite& mSprite;
  const std::string& mName;
  const std::string& mDesc;
  const uint mWorth{0};
  const ItemType mType;
};