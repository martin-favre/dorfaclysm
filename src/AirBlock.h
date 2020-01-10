#pragma once
#include "Block.h"
#include "GridMap.h"
#include "WorldItem.h"

class AirBlock : public Block {
 public:
  AirBlock() = default;
  AirBlock(const Block& block) : Block(block) {}
  const Sprite* getSprite() const override;
  bool isClickable() const override;
  bool mayPassThrough() const override;
  bool mayClimbUpFrom() const override;
  bool mayWalkOnTop() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;

  bool supportsJob(JobType type) const override;

 private:
  static const std::string mName;
};
