#pragma once
#include <algorithm>
#include <memory>

#include "MiningRequestPool.h"
#include "PlayerRequestType.h"
#include "Sprite.h"
class GridActor;
class Block {
 public:
  Block() = default;
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  virtual ~Block() = default;
  virtual const Sprite* getTopSprite() const = 0;
  virtual const Sprite* getInsideSprite() const { return getTopSprite(); }
  virtual bool isClickable() const = 0;
  virtual bool mayPassThrough() const = 0;
  virtual bool mayClimbUpFrom() const = 0;
  virtual bool mayWalkOnTop() const = 0;
  virtual const std::string& getName() const = 0;
  virtual bool supportsJob(PlayerRequestType type) const = 0;
  virtual bool isSeeThrough() const = 0;

  bool isExplored() const;
  void setExplored();

 private:
  bool mExplored{false};
};