#pragma once
#include <memory>
#include <vector>

#include "Vector3DInt.h"
#include "VisibleList.h"
class Block;
class MiningRequest {
 public:
  MiningRequest(std::weak_ptr<Block> target, const Vector3DInt &pos);
  Block &getBlock();
  bool isValid() const;
  const Vector3DInt &getPos() const;
  bool operator==(const MiningRequest &other);

 private:
  const std::weak_ptr<Block> mTarget;
  const Vector3DInt mPos;
};

class MiningRequestPool : public VisibleList<MiningRequest> {
 public:
  static void addRequest(std::unique_ptr<MiningRequest> &&job);
  static bool hasRequests();
  static std::shared_ptr<MiningRequest> getClosestTo(const Vector3DInt &pos);
  static const std::vector<std::shared_ptr<MiningRequest>>& getRequests();
  static const std::vector<std::weak_ptr<MiningRequest>>& getClaimedRequests();

 private:
  static std::vector<std::shared_ptr<MiningRequest>> mRequests;
  static std::vector<std::weak_ptr<MiningRequest>> mHandedOutRequests;
};