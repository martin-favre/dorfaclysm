#pragma once
#include <memory>

#include "Block.h"

class IPlaceableItem {
 public:
  virtual std::unique_ptr<Block> getBlock() const = 0;
};