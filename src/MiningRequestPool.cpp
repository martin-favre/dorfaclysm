#include "MiningRequestPool.h"

#include <algorithm>
#include <functional>
#include <limits>

#include "Block.h"
#include "DeltaPositions.h"
#include "GridMap.h"

MiningRequest::MiningRequest(std::weak_ptr<Block> target,
                             const Vector3DInt &pos,
                             std::function<void()> onJobCompleteCallback)
    : mTarget(target),
      mPos(pos),
      mOnJobCompleteCallback(onJobCompleteCallback) {
  ASSERT(mOnJobCompleteCallback != nullptr, "Received invalid callback");
}
Block &MiningRequest::getBlock() {
  ASSERT(isValid(), "Check if block is valid before calling");
  return *mTarget.lock();
}
MiningRequest::~MiningRequest() { mOnJobCompleteCallback(); }

bool MiningRequest::isValid() const { return !mTarget.expired(); }
const Vector3DInt &MiningRequest::getPos() const { return mPos; }
bool MiningRequest::operator==(const MiningRequest &other) const {
  ASSERT(isValid(), "Check if block is valid before calling");
  ASSERT(other.isValid(), "Check if block is valid before calling");
  const bool same = other.mTarget.lock() == mTarget.lock();
  if (same) {
    ASSERT(other.mPos == mPos,
           "Request on same block, but with different poses is not expected");
  }
  return same;
}

#if 0
bool isValidRequest(const MiningRequest &req) {
  const GridMap &map = GridMap::getActiveMap();
  for (const auto &delta : DELTA_POSITIONS_HORIZONTAL) {
    Vector3DInt pos = req.getPos() + delta;
    if (map.isPosInMap(pos)) {
      if (map.isPosFree(pos)) {
        return true;
      }
    }
  }
  return false;
}
std::vector<std::shared_ptr<MiningRequest>> MiningRequestPool::mRequests;
std::vector<std::weak_ptr<MiningRequest>> MiningRequestPool::mHandedOutRequests;

void MiningRequestPool::addRequest(std::unique_ptr<MiningRequest> &&job) {
  ASSERT(job.get(), "Received null request");
  cleanHandedoutRequests();
  auto comper = [&job](std::shared_ptr<MiningRequest> &req) {
    return *job == *req;
  };

  auto comperHandOut = [&job](std::weak_ptr<MiningRequest> &req) {
    return *job == *req.lock();
  };

  auto it = std::find_if(mRequests.begin(), mRequests.end(), comper);

  if (it == mRequests.end()) {
    auto it = std::find_if(mHandedOutRequests.begin(), mHandedOutRequests.end(),
                           comperHandOut);
    if (it == mHandedOutRequests.end()) {
      mRequests.emplace_back(std::move(job));
    }
  }
}

bool MiningRequestPool::hasRequests() { return mRequests.size(); }

std::shared_ptr<MiningRequest> MiningRequestPool::getClosestTo(
    const Vector3DInt &pos) {
  double smallestDistance = DBL_MAX;
  std::vector<std::shared_ptr<MiningRequest>>::iterator smallestIndx;
  for (auto it = mRequests.begin(); it != mRequests.end(); ++it) {
    Vector3DInt diff = pos - (*it)->getPos();
    double dist = diff.squared_magnitude();
    if (dist < smallestDistance) {
      smallestIndx = it;
      smallestDistance = dist;
    }
  }

  mHandedOutRequests.emplace_back(*smallestIndx);
  std::shared_ptr<MiningRequest> yourCopy = *smallestIndx;
  mRequests.erase(smallestIndx);
  return yourCopy;
}

const std::vector<std::shared_ptr<MiningRequest>>
    &MiningRequestPool::getRequests() {
  return mRequests;
}
const std::vector<std::weak_ptr<MiningRequest>>
    &MiningRequestPool::getClaimedRequests() {
  cleanHandedoutRequests();
  return mHandedOutRequests;
}
void MiningRequestPool::cleanHandedoutRequests() {
  for (auto it = mHandedOutRequests.begin(); it != mHandedOutRequests.end();) {
    if (it->expired()) {
      it = mHandedOutRequests.erase(it);
    } else {
      ++it;
    }
  }
}
void MiningRequestPool::returnRequest(
    std::shared_ptr<MiningRequest> &&request) {
  cleanHandedoutRequests();
  for (auto it = mHandedOutRequests.begin(); it != mHandedOutRequests.end();
       ++it) {
    {
      if (*it->lock() == *request) {
        mHandedOutRequests.erase(it);
        break;
      }
    }
  }
  mRequests.emplace_back(std::move(request));
}

#endif
MiningRequestPool MiningRequestPool::mInstance;
MiningRequestPool &MiningRequestPool::getInstance() { return mInstance; }
