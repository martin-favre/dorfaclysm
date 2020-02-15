#pragma once

#include "Block.h"
#include "GenericRequestPool.h"
class BlockBuildingRequest {
 public:
  BlockBuildingRequest(std::weak_ptr<Block> target, const Vector3DInt& pos);
  Block& getBlock();
  bool isValid() const;
  const Vector3DInt& getPos() const;
  bool operator==(const BlockBuildingRequest& other) const;

 private:
  const std::weak_ptr<Block> mTarget;
  const Vector3DInt mPos;
};

class BlockBuildingRequestPool : public GenericRequestPool<BlockBuildingRequest> {
 public:
  static BlockBuildingRequestPool& getInstance();

 private:
  BlockBuildingRequestPool() = default;
  static BlockBuildingRequestPool mInstance;
};