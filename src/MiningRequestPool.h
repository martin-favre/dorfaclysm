#pragma once
#include <memory>
#include <vector>

#include "Vector3DInt.h"
class Block;
class MiningRequest {
 public:
  MiningRequest(std::weak_ptr<Block> target, const Vector3DInt &pos);
  Block &getBlock();
  bool isValid() const;
  const Vector3DInt &getPos() const;
  bool operator==(const MiningRequest& other); 
 private:
  const std::weak_ptr<Block> mTarget;
  const Vector3DInt mPos;
};

class MiningRequestPool {
 public:
  static void addRequest(std::unique_ptr<MiningRequest> &&job);
  static const std::vector<std::unique_ptr<MiningRequest>> &getJobs();
  static std::unique_ptr<MiningRequest> claimRequest(
      std::vector<std::unique_ptr<MiningRequest>>::const_iterator whichJob);

 private:
  static std::vector<std::unique_ptr<MiningRequest>> mJobs;
};