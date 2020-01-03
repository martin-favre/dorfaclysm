#include "TileFloor.h"
namespace {
std::unique_ptr<Sprite> grassSprite;

std::unique_ptr<Sprite> dirtSprite;

std::unique_ptr<Sprite> rockSprite;

std::unique_ptr<Sprite> openSprite;

void loadAllSprites()
{
  if(!grassSprite.get())
  {
    grassSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {2, 1},
                                                  Paths::SIZE_OF_GRASS_TILE);
    dirtSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {0, 2},
                                                 Paths::SIZE_OF_GRASS_TILE);
    rockSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {1, 6},
                                                 Paths::SIZE_OF_GRASS_TILE);
    openSprite = SpriteLoader::loadSpriteByIndex(Paths::GRASS_TILE, {5, 5},
                                                 Paths::SIZE_OF_GRASS_TILE);

  }
}


Sprite& decideSprite(TileFloor::TileFloorType type) {
  loadAllSprites();
  switch (type) {
    case TileFloor::grass:
      return *grassSprite;
    case TileFloor::dirt:
      return *dirtSprite;
    case TileFloor::rock:
      return *rockSprite;
    case TileFloor::open:
      return *openSprite;
    default:
      ASSERT(false, "Unknown TileFloorType");
      return *grassSprite;
  }
}

bool decideCanMoveOver(TileFloor::TileFloorType type) {
  switch (type) {
    case TileFloor::grass:
    case TileFloor::dirt:
      return true;
    case TileFloor::rock:
    case TileFloor::open:
      return false;
    default:
      ASSERT(false, "Unknown TileFloorType");
      return false;
  }
}

std::string decideName(TileFloor::TileFloorType type) {
  switch (type) {
    case TileFloor::grass:
      return "Grass";
    case TileFloor::dirt:
      return "Dirt";
    case TileFloor::rock:
      return "Rock Floor";
    case TileFloor::open:
      return "Open Air";
    default:
      ASSERT(false, "Unknown TileFloorType");
      return "";
  }
}
}  // namespace

TileFloor::TileFloor(TileFloorType type)
    : TileFloor(decideSprite(type), decideName(type), decideCanMoveOver(type)) {
}
