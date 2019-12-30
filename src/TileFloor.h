#pragma once
#include <memory>

#include "Sprite.h"
#include "SpriteLoader.h"
class TileFloor {
 public:
  TileFloor(const Sprite& sprite, bool canMoveOver = true)
      : mSprite(sprite), mCanMoveOver(canMoveOver) {}
  TileFloor(const TileFloor&) = default;
  TileFloor& operator=(const TileFloor&) = default;
  inline bool canMoveOver() const { return mCanMoveOver; }
  const inline Sprite& getSprite() const { return mSprite; }
  int getMovementCost() const { return 0; }  // todo
 private:
  const Sprite& mSprite;
  bool mCanMoveOver{true};
};

TileFloor getGrassTileFloor();
TileFloor getDirtTileFloor();
TileFloor getRockTileFloor();
TileFloor getOpenTileFloor();
