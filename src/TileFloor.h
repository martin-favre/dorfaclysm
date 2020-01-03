#pragma once
#include <memory>

#include "Sprite.h"
#include "SpriteLoader.h"
class TileFloor {
 public:
  enum TileFloorType{grass, dirt, rock, open};
  TileFloor(TileFloorType type);
  TileFloor(Sprite& sprite, const std::string& name, bool canMoveOver = true)
      : mSprite(sprite), mCanMoveOver(canMoveOver), mName(name) {}
  TileFloor(const TileFloor&) = default;
  TileFloor& operator=(const TileFloor&) = default;
  inline bool canMoveOver() const { return mCanMoveOver; }
  inline const Sprite& getSprite() const { return mSprite; }
  inline const std::string& getName() const {return mName; } 
  int getMovementCost() const { return 0; }  // todo
 private:
  Sprite& mSprite;
  bool mCanMoveOver{true};
  std::string mName;
};
