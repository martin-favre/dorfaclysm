#include "DorfWalker.h"

#include "Astar.h"
#include "GridMap.h"
Vector2DInt DorfWalker::getNextPlannedPosition() {
  if (!mPlannedPosititions.empty()) {
    return mPlannedPosititions.top();
  } else {
    return {0, 0};
  }
}

void DorfWalker::walkUpdate(Vector2DInt& currentPosition) {
  if (!mPlannedPosititions.empty()) {
    if (mTimer.getElapsedMilliseconds() > mMsPerMovement) {
      Vector2DInt next = mPlannedPosititions.top();
      if (GridMap::getActiveMap().isPosFree(next)) {
        currentPosition = next;
        mPlannedPosititions.pop();
      }
      mTimer.start();
    }
  }
}

bool DorfWalker::isDone() { return mPlannedPosititions.empty(); }

bool DorfWalker::generateNewPath(const Vector2DInt& from,
                                 const Vector2DInt& to) {
  mTimer.start();
  return Astar().getPath(from, to, GridMap::getActiveMap(),
                         mPlannedPosititions);
}