#pragma once

#include "Block.h"
#include "BlockIdentifier.h"
#include "GenericRequestPool.h"
#include "Serializer.h"
class BlockBuildingRequest {
 public:
  BlockBuildingRequest(const SerializedObj& serObj);
  BlockBuildingRequest(const BlockIdentifier& targetBlock,
                       const Vector3DInt& pos);
  const BlockIdentifier& getBlockIdentifier() const;
  bool isValid() const;
  const Vector3DInt& getPos() const;
  bool operator==(const BlockBuildingRequest& other) const;
  
 private:
  const BlockIdentifier mTargetBlock;
  const Vector3DInt mPos;
};

void to_json(SerializedObj& out, const BlockBuildingRequest& req);

class BlockBuildingRequestPool
    : public GenericRequestPool<BlockBuildingRequest> {
 public:
  static BlockBuildingRequestPool& getInstance();

 private:
  BlockBuildingRequestPool() = default;
  static BlockBuildingRequestPool mInstance;
};