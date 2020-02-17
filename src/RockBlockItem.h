#pragma once
#include "IPlaceableItem.h"
#include "Item.h"
#include "Sprite.h"
class RockBlockItem : public Item, public IPlaceableItem {
 public:
  const std::string& getName() const override;
  const std::string& getDescription() const override;
  const Sprite& getSprite() const override;
  uint getWorth() const override;
  const ItemType& getItemType() const override;

  std::unique_ptr<Block> getBlock() const override;
  bool isPlaceable() const override;

 private:
  const Sprite& loadSprite() const;
  static std::unique_ptr<Sprite> mSprite;
  static const std::string mName;
  static const std::string mDesc;
  static const uint mWorth;
  static const ItemType mType;
};