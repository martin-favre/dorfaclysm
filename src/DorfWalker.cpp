#include "DorfWalker.h"

#include "Astar.h"
#include "GridMap.h"
Vector3DInt DorfWalker::getNextPlannedPosition() {
  if (!mPlannedPosititions.empty()) {
    return mPlannedPosititions.top();
  } else {
    return {0, 0};
  }
}

void DorfWalker::walkUpdate(Vector3DInt& currentPosition) {
  if (!mPlannedPosititions.empty()) {
    if (mTimer.getElapsedMilliseconds() > mMsPerMovement) {
      Vector3DInt next = mPlannedPosititions.top();
      if (GridMap::getActiveMap().isPosFree(next)) {
        currentPosition = next;
        mPlannedPosititions.pop();
      }
      mTimer.start();
    }
  }
}

bool DorfWalker::isDone() { return mPlannedPosititions.empty(); }

bool DorfWalker::generateNewPath(const Vector3DInt& from,
                                 const Vector3DInt& to) {
  mTimer.start();
  return Astar().getPath(from, to, GridMap::getActiveMap(),
                         mPlannedPosititions);
}