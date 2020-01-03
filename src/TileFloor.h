#pragma once
#include <memory>

#include "Sprite.h"
#include "SpriteLoader.h"
class TileFloor {
 public:
  TileFloor(std::unique_ptr<Sprite>&& sprite, const std::string& name,
            bool canMoveOver = true)
      : mSprite(std::move(sprite)), mCanMoveOver(canMoveOver), mName(name) {}
  TileFloor(const TileFloor&) = default;
  TileFloor& operator=(const TileFloor&) = default;
  inline bool canMoveOver() const { return mCanMoveOver; }
  inline const Sprite& getSprite() const {
    ASSERT(mSprite.get(), "sprite is null");
    return *mSprite;
  }
  inline const std::string& getName() const { return mName; }
  int getMovementCost() const { return 0; }  // todo
 private:
  std::unique_ptr<Sprite> mSprite;
  bool mCanMoveOver{true};
  std::string mName;
};

class GrassTileFloor : public TileFloor {
 public:
  GrassTileFloor()
      : TileFloor(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {2, 1},
                                                  Paths::SIZE_OF_GRASS_TILE),
                  "Grass") {}
};

class DirtTileFloor : public TileFloor {
 public:
  DirtTileFloor()
      : TileFloor(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {0, 2},
                                                  Paths::SIZE_OF_GRASS_TILE),
                  "Dirt") {}
};

class RockTileFloor : public TileFloor {
 public:
  RockTileFloor()
      : TileFloor(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1, 6},
                                                  Paths::SIZE_OF_GRASS_TILE),
                  "Rock Floor") {}
};