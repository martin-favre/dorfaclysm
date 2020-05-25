#include "DorfWalker.h"

#include "Astar.h"
#include "GridMap.h"
#include "Vector3DInt.h"


DorfWalker::DorfWalker(GridActor& gridActor, int msPerMovement)
    : mGridActor(gridActor),
      mGridMap(GridMap::getActiveMap()),
      mMsPerMovement(msPerMovement) {}

DorfWalker::DorfWalker(GridActor& gridActor, const SerializedObj& serObj)
    : mGridActor(gridActor),
      mGridMap(GridMap::getActiveMap()),
      mState(serObj.at("state")),
      mFail(serObj.at("fail")),
      mMsPerMovement(serObj.at("speed")),
      mGoal(serObj.at("goal")),
      mTimer(serObj.at("timer")),
      mPlannedPosititions(serObj.at("plannedPositions"))

{
  // mPlannedPosititions = serObj.at("plannedPositions");
}

void to_json(SerializedObj& serObj, const DorfWalker& walker) {
  serObj["state"] = walker.mState;
  serObj["fail"] = walker.mFail;
  std::vector<Vector3DInt> positions; 
  std::stack<Vector3DInt> stackPos = walker.mPlannedPosititions;
  while(!stackPos.empty()){
    positions.emplace_back(stackPos.top());
    stackPos.pop();
  }
  serObj["plannedPositions"] = positions;
  serObj["speed"] = walker.mMsPerMovement;
  serObj["goal"] = walker.mGoal;
  serObj["timer"] = walker.mTimer;
}

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
