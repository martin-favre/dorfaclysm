#pragma once
#include "Block.h"

class Item;
class AirBlock : public Block {
 public:
  AirBlock() = default;
  const Sprite* getTopSprite() const override;
  bool mayPassThrough() const override;
  bool mayClimbUpFrom() const override;
  bool mayWalkOnTop() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;
  bool spawnsItem() const override;
  bool supportsJob(PlayerRequestType type) const override;
  std::unique_ptr<Item> getItem() const override;
  BlockType getType() const override;
 private:
  static const std::string mName;
};
