#pragma once
#include "IPlaceableItem.h"
#include "Item.h"

class RockBlockItem : public Item, public IPlaceableItem {
 public:
  RockBlockItem();
  std::unique_ptr<Block> getBlock() const override;
  bool isPlaceable() const override;

 private:
  const Sprite& loadSprite() const;
  static std::unique_ptr<Sprite> mSprite;
};