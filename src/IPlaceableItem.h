#pragma once
#include <memory>

#include "Block.h"

class IPlaceableItem {
 public:
  virtual BlockType getBlock() const = 0;
};