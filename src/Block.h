#pragma once
#include <memory>

#include "JobPool.h"
#include "Sprite.h"
#include "WorldItemComponent.h"
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

enum BlockMovementType
{
  movementOnlyHorizontal,
  movementUpBlock,
  movementDownBlock,
  movementUpAndDownBlock
};

class Block {
 public:
  virtual const Sprite* getSprite() const = 0;
  virtual bool isClickable() const = 0;
  virtual bool isPassable() const = 0;
  virtual const std::string& getName() const = 0;
  virtual bool supportsJob(JobType type) const = 0;
  virtual BlockMovementType getMovementType() const = 0;
  virtual bool isSeeThrough() const {return false;};
  virtual const std::vector<WorldItemComponent>& getComponents()const {return mNoComponents;}
  void assignJob(JobType type);
  void unassignJob();
  JobType getJob() const;

 private:
  static const std::vector<WorldItemComponent> mNoComponents;
  JobType mAssignedJob{jobTypeUnset};
};

const std::vector<WorldItemComponent> Block::mNoComponents;
