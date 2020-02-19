#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "GenericRequestPool.h"
#include "Vector3DInt.h"

class Block;
class MiningRequest {
 public:
  MiningRequest(std::weak_ptr<Block> target, const Vector3DInt& pos, std::function<void()> onJobCompleteCallback);
  MiningRequest(const MiningRequest&) = delete;
  MiningRequest(const MiningRequest&&) = delete;
  const MiningRequest& operator=(const MiningRequest&) = delete;
  const MiningRequest& operator=(const MiningRequest&&) = delete;
  ~MiningRequest();
  Block& getBlock();
  bool isValid() const;
  const Vector3DInt& getPos() const;
  bool operator==(const MiningRequest& other) const;

 private:
  const std::weak_ptr<Block> mTarget;
  const Vector3DInt mPos;
  std::function<void()> mOnJobCompleteCallback;
};

class MiningRequestPool : public GenericRequestPool<MiningRequest> {
 public:
  static MiningRequestPool& getInstance();

 private:
  MiningRequestPool() = default;
  static MiningRequestPool mInstance;
};
