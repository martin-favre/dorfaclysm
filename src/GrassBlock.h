#pragma once
#include "Block.h"

class GrassBlock : public Block {
  public:
  GrassBlock();
  const Sprite* getSprite() const override;
  bool isClickable() const override;
  bool isPassable() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;
  bool supportsJob(JobType type) const override;
  BlockMovementType getMovementType() const override;

 private:
  static std::unique_ptr<Sprite> mSprite;
  static const std::string mName;
};