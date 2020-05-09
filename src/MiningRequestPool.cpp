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
MiningRequestPool MiningRequestPool::mInstance;
MiningRequestPool &MiningRequestPool::getInstance() { return mInstance; }
