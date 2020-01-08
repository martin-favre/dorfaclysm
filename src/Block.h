#pragma once
#include <algorithm>
#include <memory>

#include "JobPool.h"
#include "Sprite.h"

/**
 * a. Blocks decide on movement based on neighbours
 *    You can only go up if the block below you is ground
 *    Stairs/ramps increase speed
 * b. Block decide on movement based on type
 *    BasicBlock does not allow you to go up
 *    Stair/Rampblock allows you to go up/down
 *      (but open air with a ramp below it allows you to go down?)
 *    Maybe you need both
 */

enum BlockMovementType {
  movementOnlyHorizontal,
  movementUpBlock,
  movementDownBlock,
  movementUpAndDownBlock
};
class WorldItem;
class Block {
 public:
  virtual const Sprite* getSprite() const = 0;
  virtual bool isClickable() const = 0;
  virtual bool isPassable() const = 0;
  virtual const std::string& getName() const = 0;
  virtual bool supportsJob(JobType type) const = 0;
  virtual BlockMovementType getMovementType() const = 0;
  virtual bool isSeeThrough() const { return false; };
  virtual bool supportsComponents() const { return false; }

  const std::vector<WorldItem*>& getComponents() const;

  void registerComponent(WorldItem& comp);

  void unregisterComponent(WorldItem& comp);

  void assignJob(JobType type);
  void unassignJob();
  JobType getJob() const;

 private:
  std::vector<WorldItem*> mComponents;  // This is gonna take up a lot of memory
  JobType mAssignedJob{jobTypeUnset};
};