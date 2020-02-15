#include "BlockBuildingRequestPool.h"

BlockBuildingRequest::BlockBuildingRequest(std::weak_ptr<Block> target,
                                   const Vector3DInt& pos)
    : mTarget(target), mPos(pos) {}
Block& BlockBuildingRequest::getBlock() {
  ASSERT(isValid(), "Check if block is valid before calling");
  return *mTarget.lock();
}
bool BlockBuildingRequest::isValid() const { return !mTarget.expired(); }

const Vector3DInt& BlockBuildingRequest::getPos() const { return mPos; }

bool BlockBuildingRequest::operator==(const BlockBuildingRequest& other) const {
  ASSERT(isValid(), "Check if block is valid before calling");
  ASSERT(other.isValid(), "Check if block is valid before calling");
  const bool same = other.mTarget.lock() == mTarget.lock();
  if (same) {
    ASSERT(other.mPos == mPos,
           "Request on same block, but with different poses is not expected");
  }
  return same;
}
BlockBuildingRequestPool& BlockBuildingRequestPool::getInstance() { return mInstance; }

BlockBuildingRequestPool BlockBuildingRequestPool::mInstance;