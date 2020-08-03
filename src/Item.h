#pragma once
#include <string>
#include <typeindex>

#include "BlockType.h"
#include "ItemType.h"
#include "Serializer.h"
#include "Sprite.h"
class Item {
 public:
  Item(const SerializedObj& serObj);
  constexpr Item(ItemType type) : mType(type) {}
  Item(Item&&) = default;
  const std::string& getName() const;
  const std::string& getDescription() const;
  const Sprite& getSprite() const;
  uint getWorth() const;
  ItemType getItemType() const;
  BlockType getBlockType() const;
  bool isPlaceable() const;
  bool isValid() const;
  uint32_t getCount() const;
  bool isEmpty() const;
  void addItems(uint32_t count);
  void removeItems(uint32_t count);
  static constexpr Item invalidItem() { return Item(ItemType_Invalid); }

 private:
  static void initialize();
  static bool mInitialized;

  ItemType mType;
  uint32_t mCount{1};
};

void to_json(SerializedObj& out, const Item& item);
