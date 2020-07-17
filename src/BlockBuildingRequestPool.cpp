#include "BlockBuildingRequestPool.h"

#include "GridMap.h"

BlockBuildingRequest::BlockBuildingRequest(const SerializedObj& serObj)
    : mTargetBlock(serObj["identifier"]), mPos(serObj["position"]) {}

BlockBuildingRequest::BlockBuildingRequest(const BlockIdentifier& targetBlock,
                                           const Vector3DInt& pos)
    : mTargetBlock(targetBlock), mPos(pos) {}

const BlockIdentifier& BlockBuildingRequest::getBlockIdentifier() const {
  return mTargetBlock;
}

bool BlockBuildingRequest::isValid() const {
  return GridMap::getActiveMap().blockIdentifierMatches(mTargetBlock, mPos);
}

const Vector3DInt& BlockBuildingRequest::getPos() const { return mPos; }

bool BlockBuildingRequest::operator==(const BlockBuildingRequest& other) const {
  const bool same = other.mTargetBlock == mTargetBlock;
  if (same) {
    ASSERT(other.mPos == mPos,
           "Request on same block, but with different poses is not expected");
  }
  return same;
}

void to_json(SerializedObj& out, const BlockBuildingRequest& bbReq) {
  out["identifier"] = bbReq.getBlockIdentifier();
  out["position"] = bbReq.getPos();
}

BlockBuildingRequestPool& BlockBuildingRequestPool::getInstance() {
  return mInstance;
}

BlockBuildingRequestPool BlockBuildingRequestPool::mInstance;