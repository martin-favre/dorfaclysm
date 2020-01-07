#pragma once
#include "Block.h"
#include "GridMap.h"
#include "WorldItemComponent.h"

class AirBlock : public Block {
  public:
  const Sprite* getSprite() const override;
  bool isClickable() const override;
  bool isPassable() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;
  const std::vector<WorldItemComponent>& getComponents()const override;

  bool supportsJob(JobType type) const override;
  BlockMovementType getMovementType() const override;

 private:
  static const std::string mName;
  std::vector<WorldItemComponent> mComponents;
};
