// tmp code to test astar

#include <chrono>
#include <thread>

#include "Astar.h"
#include "GraphicsManager.h"
#include "GridMap.h"
#include "Helpers.h"
#include "Timer.h"
/*
class OpenTile : public Tile {
 public:
  OpenTile() { mOpen = true; }
};

class SnekTile : public Tile {
 public:
  SnekTile() { mOpen = false; }
};

class SnekPiece {
 public:
  SnekPiece(const Vector2DInt& pos) : mPos(pos) {}

  void moveTo(const Vector2DInt& position, GridMap& map) {
    const Vector2DInt oldPos = mPos;
    mPos = position;
    map.setTile(mPos, SnekTile());
    if (mChild) {
      mChild->moveTo(oldPos, map);
    } else if (mShouldSpawnTail) {
      mChild = std::make_unique<SnekPiece>(oldPos);
    } else {
      map.setTile(mPos, OpenTile());
    }
  }

  void spawnTail() {
    if (mChild) {
      mChild->spawnTail();
    } else {
      mShouldSpawnTail = true;
    }
  }

  void render() {
    GraphicsManager::drawRect(
        Rect{mPos.x * mSize.x, mPos.y * mSize.y, mSize.x, mSize.y});
    if (mChild) {
      mChild->render();
    }
  }

  bool mShouldSpawnTail{false};
  std::unique_ptr<SnekPiece> mChild{nullptr};
  Vector2DInt mPos;
  const Vector2DInt mSize{10, 10};
};

class Snek {
 public:
  bool recalculatePath(const Vector2DInt& targetPos, const GridMap& map) {
    mPath = std::stack<Vector2DInt>();
    return Astar().getPath(mSnekHead.mPos, targetPos, map, mPath);
  }

  void move(GridMap& map) {
    if (!mPath.empty()) {
      Vector2DInt newPos = mPath.top();
      mPath.pop();
      mSnekHead.moveTo(newPos, map);
    }
  }

  bool atTarget() { return mPath.empty(); }

  std::stack<Vector2DInt> mPath;
  SnekPiece mSnekHead{Vector2DInt(50, 50)};
};

int main() {
  GraphicsManager::initialize();
  const Vector2DInt mapSize(150, 80);
  GridMap::generateActiveMap(mapSize, nullptr);
  GridMap& map = GridMap::getActiveMap();
  std::unique_ptr<Snek> snek = std::make_unique<Snek>();
  Vector2DInt targetPos;
  while (true) {
    GraphicsManager::prepareRendering();
    if (snek->atTarget()) {
      snek->mSnekHead.spawnTail();
      do {
        int randX = Helpers::randomInt(0, mapSize.x - 1);
        int randY = Helpers::randomInt(0, mapSize.y - 1);
        targetPos = Vector2DInt(randX, randY);
      } while ((targetPos == snek->mSnekHead.mPos) ||
               !map.getTile(targetPos).isOpen());
    }
    GraphicsManager::setRenderDrawColor(SDL_Color{255, 0, 0, 255});
    GraphicsManager::drawRect(Rect{targetPos.x * snek->mSnekHead.mSize.x,
                                   targetPos.y * snek->mSnekHead.mSize.x, 5,
                                   5});
    bool success = snek->recalculatePath(targetPos, map);
    snek->move(map);
    GraphicsManager::setRenderDrawColor(SDL_Color{0, 255, 0, 255});
    snek->mSnekHead.render();
    GraphicsManager::executeRendering();
    SDL_Delay(0);
    if (!success) {
      GridMap::generateActiveMap(mapSize, nullptr);
      snek = std::make_unique<Snek>();
    }
  }
}
*/