#pragma once
#include <memory>
#include <vector>

#include "GenericRequestPool.h"
#include "Vector3DInt.h"

class Block;
class MiningRequest {
 public:
  MiningRequest(std::weak_ptr<Block> target, const Vector3DInt& pos);
  ~MiningRequest() 
  {
    std::cout << "Ech" << std::endl;
  }
  Block& getBlock();
  bool isValid() const;
  const Vector3DInt& getPos() const;
  bool operator==(const MiningRequest& other) const;

 private:
  const std::weak_ptr<Block> mTarget;
  const Vector3DInt mPos;
};

class MiningRequestPool : public GenericRequestPool<MiningRequest> {
 public:
  static MiningRequestPool& getInstance();

 private:
  MiningRequestPool() = default;
  static MiningRequestPool mInstance;
};
