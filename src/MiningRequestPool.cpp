#include "MiningRequestPool.h"

#include <algorithm>
#include <functional>
#include <limits>

#include "Block.h"
#include "GridMap.h"

std::vector<std::shared_ptr<MiningRequest>> MiningRequestPool::mRequests;
std::vector<std::weak_ptr<MiningRequest>> MiningRequestPool::mHandedOutRequests;

MiningRequest::MiningRequest(std::weak_ptr<Block> target,
                             const Vector3DInt &pos)
    : mTarget(target), mPos(pos) {}
Block &MiningRequest::getBlock() { return *mTarget.lock(); }
bool MiningRequest::isValid() const { return !mTarget.expired(); }
const Vector3DInt &MiningRequest::getPos() const { return mPos; }
bool MiningRequest::operator==(const MiningRequest &other) {
  const bool same = other.mTarget.lock() == mTarget.lock();
  if (same) {
    ASSERT(other.mPos == mPos,
           "Request on same block, but with different poses is not expected");
  }
  return same;
}

void MiningRequestPool::addRequest(std::unique_ptr<MiningRequest> &&job) {
  ASSERT(job.get(), "Received null request");
  // avoid duplicates
  auto comper = [&job](std::shared_ptr<MiningRequest> &req) {
    return *job == *req;
  };
  auto it = std::find_if(mRequests.begin(), mRequests.end(), comper);
  if (it == mRequests.end()) {
    mRequests.emplace_back(std::move(job));
  }
}

bool MiningRequestPool::hasRequests() { return mRequests.size(); }

std::shared_ptr<MiningRequest> MiningRequestPool::getClosestTo(
    const Vector3DInt &pos)

{
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
  for (auto it = mHandedOutRequests.begin(); it != mHandedOutRequests.end();) {
    if (it->expired()) {
      it = mHandedOutRequests.erase(it);
    } else {
      ++it;
    }
  }

  return mHandedOutRequests;
}
