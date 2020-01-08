#pragma once
#include "Block.h"
#include "GridMap.h"
#include "WorldItem.h"

class AirBlock : public Block {
  public:
  const Sprite* getSprite() const override;
  bool isClickable() const override;
  bool isPassable() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;
  bool supportsComponents() const override;

  bool supportsJob(JobType type) const override;
  BlockMovementType getMovementType() const override;

 private:
  static const std::string mName;
  std::vector<WorldItem*> mComponents;
};
