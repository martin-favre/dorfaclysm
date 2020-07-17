#pragma once
#include "Block.h"
class Item;
class RockBlock : public Block {
 public:
  RockBlock();
  const Sprite* getTopSprite() const override;
  bool mayPassThrough() const override;
  bool mayClimbUpFrom() const override;
  bool mayWalkOnTop() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;
  bool supportsJob(PlayerRequestType type) const override;
  bool spawnsItem() const override;
  std::unique_ptr<Item> getItem() const override;
  
 private:
  static std::unique_ptr<Sprite> mSprite;
  static const std::string mName;
};