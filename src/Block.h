#pragma once
#include <memory>

#include "BlockIdentifier.h"
#include "BlockType.h"
#include "PlayerRequestType.h"
#include "Serializer.h"

class GridActor;
class Item;
class Sprite;

class Block {
 public:
  Block(const BlockIdentifier& identifier);
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  virtual ~Block() = default;
  virtual const Sprite* getTopSprite() const = 0;
  virtual bool mayPassThrough() const = 0;
  virtual bool mayClimbUpFrom() const = 0;
  virtual bool mayWalkOnTop() const = 0;
  virtual const std::string& getName() const = 0;
  virtual bool supportsJob(PlayerRequestType type) const = 0;
  virtual bool isSeeThrough() const = 0;
  virtual bool spawnsItem() const = 0;
  virtual std::unique_ptr<Item> getItem() const = 0;
  const BlockIdentifier& getIdentifier() const;

  bool isExplored() const;
  void setExplored();

 private:
  bool mExplored{false};
  const BlockIdentifier mIdentifier;
};