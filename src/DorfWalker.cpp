#include "DorfWalker.h"

#include "Astar.h"
#include "GridMap.h"

DorfWalker::DorfWalker(GridActor& gridActor, int msPerMovement)
    : mMsPerMovement(msPerMovement),
      mGridActor(gridActor),
      mGridMap(GridMap::getActiveMap()) {}

Vector3DInt DorfWalker::getNextPlannedPosition() {
  if (!mPlannedPosititions.empty()) {
    return mPlannedPosititions.top();
  } else {
    return {0, 0};
  }
}

void DorfWalker::update() {
  switch (mState) {
    case calculatingPath:
      calculateNewPath();
      break;
    case walking:
      walk();
      break;
    default:
      break;
  }
}

void DorfWalker::walk() {
  if (!mPlannedPosititions.empty()) {
    if (mTimer.getElapsedMilliseconds() > mMsPerMovement) {
      Vector3DInt next = mPlannedPosititions.top();
      if (GridMap::getActiveMap().isPosFree(next)) {
        mGridActor.moveTo(next);
        mPlannedPosititions.pop();
      } else {
        mState = idle;
        mFail = pathInterrupted;
        std::stack<Vector3DInt> s;
        mPlannedPosititions.swap(s);
      }
      mTimer.start();
    }
  } else {
    mState = idle;
  }
}

bool DorfWalker::isCalculating() const { return mState == calculatingPath; }
bool DorfWalker::isWalking() const { return mState == walking; }
bool DorfWalker::isIdle() const { return mState == idle; }
bool DorfWalker::hasFailed() const { return mFail != noFailure; }

void DorfWalker::startNewPath(const Vector3DInt& to) {
  mAstar = std::make_unique<IncrementalAstar>(mGridActor.getCurrentPos(), to,
                                              mGridMap);
  mGoal = to;
  mState = calculatingPath;
  mFail = noFailure;
}

DorfWalker::FailureReason DorfWalker::getFailReason() const { return mFail; }

void DorfWalker::calculateNewPath() {
  ASSERT(mAstar.get(), "Must call startNewPath first");
  mAstar->calculatePath();
  if (mAstar->isDone()) {
    if (mAstar->foundPath()) {
      mAstar->getPath(mPlannedPosititions);
      mAstar.reset();
      mState = walking;
    } else {
      mState = idle;
      mFail = noPathFound;
    }
  }
}