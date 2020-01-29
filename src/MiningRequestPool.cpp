#include "MiningRequestPool.h"

#include <algorithm>
#include <functional>
#include "Block.h"
#include "GridMap.h"

std::vector<std::unique_ptr<MiningRequest>> MiningRequestPool::mJobs;

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
           "Request on same block but with different poses not expected");
  }
  return same;
}

void MiningRequestPool::addRequest(std::unique_ptr<MiningRequest> &&job) {
  // avoid duplicates
  auto comper = [&job](std::unique_ptr<MiningRequest> &req) {
    return job == req;
  };
  auto it = std::find_if(mJobs.begin(), mJobs.end(), comper);
  if (it == mJobs.end()) {
    mJobs.emplace_back(std::move(job));
  }
}

const std::vector<std::unique_ptr<MiningRequest>>
    &MiningRequestPool::getJobs() {
  return mJobs;
}

std::unique_ptr<MiningRequest> MiningRequestPool::claimRequest(
    std::vector<std::unique_ptr<MiningRequest>>::const_iterator whichJob) {
  ASSERT(whichJob != mJobs.end(), "Job iter out of bounds");
  int index = whichJob - mJobs.begin();
  std::unique_ptr<MiningRequest> job = std::move(mJobs[index]);
  mJobs.erase(whichJob);
  return job;
}
