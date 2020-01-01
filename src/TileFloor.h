#pragma once
#include <memory>

#include "Sprite.h"
#include "SpriteLoader.h"
class TileFloor {
 public:
  TileFloor(const Sprite& sprite, const std::string& name, bool canMoveOver = true)
      : mSprite(sprite), mCanMoveOver(canMoveOver), mName(name) {}
  TileFloor(const TileFloor&) = default;
  TileFloor& operator=(const TileFloor&) = default;
  inline bool canMoveOver() const { return mCanMoveOver; }
  inline const Sprite& getSprite() const { return mSprite; }
  inline const std::string& getName() const {return mName; } 
  int getMovementCost() const { return 0; }  // todo
 private:
  const Sprite& mSprite;
  const bool mCanMoveOver{true};
  const std::string mName;
};

TileFloor getGrassTileFloor();
TileFloor getDirtTileFloor();
TileFloor getRockTileFloor();
TileFloor getOpenTileFloor();
