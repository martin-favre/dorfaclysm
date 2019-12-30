
#pragma once
#include <memory>

#include "Sprite.h"
#include "SpriteLoader.h"
class Tile {
 public:
  virtual bool isOpen() const { return false; }
  virtual const Sprite& getSprite() const = 0;
};

class RockTile : public Tile {
 public:
  RockTile()
      : mSprite(SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1, 2},
                                                Paths::SIZE_OF_GRASS_TILE)) {}
  const Sprite& getSprite() const override { return *mSprite; }

 private:
  std::unique_ptr<Sprite> mSprite;
};