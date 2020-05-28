#pragma once

#include "Block.h"
#include "Item.h"
// for unit tests
class DefaultBlock : public Block {
 public:
  const Sprite* getTopSprite() const override { return nullptr; }
  bool mayPassThrough() const override { return false; }
  bool mayClimbUpFrom() const override { return false; }
  bool mayWalkOnTop() const override { return false; }
  const std::string& getName() const override { return mName; }
  bool supportsJob(PlayerRequestType type) const override {
    (void)type;
    return false;
  };
  bool isSeeThrough() const override { return false; };
  bool spawnsItem() const override { return false; }
  std::unique_ptr<Item> getItem() const override { return nullptr; }
  BlockType getType() const override { return BlockTypeInvalid;};

 private:
  const std::string mName;
};