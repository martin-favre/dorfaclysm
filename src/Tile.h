
#pragma once
#include <memory>

#include "JobPool.h"
#include "Sprite.h"
#include "SpriteLoader.h"

class Tile {
 public:
  Tile(const std::string& name) : mName(name) {}
  virtual bool isOpen() const { return false; }
  virtual bool supportsJob(PlayerRequestedJob::JobType type) const {
    (void)type;
    return false;
  }
  void assignJob(PlayerRequestedJob::JobType type) { mAssignedJob = type; }

  void unAssignJob() { mAssignedJob = PlayerRequestedJob::unset; }

  PlayerRequestedJob::JobType getJob() { return mAssignedJob; }

  virtual const Sprite& getSprite() const = 0;
  inline const std::string& getName() const { return mName; }

 private:
  PlayerRequestedJob::JobType mAssignedJob{PlayerRequestedJob::unset};
  const std::string mName;
};

class RockTile : public Tile {
 public:
  RockTile()
      : Tile("Rock Wall"),
        mSprite(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1, 2},
                                                Paths::SIZE_OF_GRASS_TILE)) {}
  const Sprite& getSprite() const override { return *mSprite; }
  bool supportsJob(PlayerRequestedJob::JobType type) const override {
    return type == PlayerRequestedJob::JobType::mine;
  }

 private:
  std::unique_ptr<Sprite> mSprite;
};