#pragma once
#include <memory>

#include "Block.h"

class IPlaceableItem {
  virtual std::unique_ptr<Block> getBlock() const = 0;
};