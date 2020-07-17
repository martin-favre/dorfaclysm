#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "GenericRequestPool.h"
#include "Vector3DInt.h"
#include "MiningRequest.h"

class Block;
class MiningRequestPool : public GenericRequestPool<MiningRequest> {
 public:
  static MiningRequestPool& getInstance();

 private:
  MiningRequestPool() = default;
  static MiningRequestPool mInstance;
};
