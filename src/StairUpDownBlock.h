#pragma once
#include "Block.h"
class StairUpDownBlock : public Block {
 public:
  StairUpDownBlock();
  const Sprite* getSprite() const override;
  bool isClickable() const override;
  bool isPassable() const override;
  const std::string& getName() const override;
  bool supportsJob(JobType type) const override;
  BlockMovementType getMovementType() const override;

 private:
  static std::unique_ptr<Sprite> mSprite;
  static const std::string mName;
};