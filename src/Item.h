#pragma once
#include <string>

#include "Sprite.h"

class Item {
 public:
  const std::string& getName() const;
  const std::string& getDescription() const;
  const Sprite& getSprite() const;
  uint getWorth() const;

  // If true, then it also inherits from IPlaceableItem
  virtual bool isPlaceable() const = 0;

 protected:
  Item(const Sprite& sprite, const std::string& name, const std::string& desc,
       const uint worth);

 private:
  const Sprite& mSprite;
  const std::string& mName;
  const std::string& mDesc;
  const uint mWorth{0};
};