#pragma once
#include "Block.h"
#include "GridActor.h"
#include "GridMap.h"

class AirBlock : public Block {
 public:
  AirBlock() = default;
  const Sprite* getTopSprite() const override;
  bool isClickable() const override;
  bool mayPassThrough() const override;
  bool mayClimbUpFrom() const override;
  bool mayWalkOnTop() const override;
  bool isSeeThrough() const override;
  const std::string& getName() const override;

  bool supportsJob(PlayerRequestType type) const override;

 private:
  static const std::string mName;
};
