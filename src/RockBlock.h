#pragma once
#include "Block.h"
class RockBlock : public Block {
 public:
  RockBlock();
  virtual const Sprite* getSprite() const;
  virtual bool isClickable() const;
  virtual bool isPassable() const;
  virtual const std::string& getName() const;
  virtual bool supportsJob(JobType type) const;
  virtual BlockMovementType getMovementType() const;

 private:
  static std::unique_ptr<Sprite> mSprite;
  static const std::string mName;
};