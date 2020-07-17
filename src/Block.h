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
  Block(BlockType);
  Block(const BlockIdentifier& identifier);
  Block(const SerializedObj& block);
  ~Block() = default;
  const Sprite* getTopSprite() const;
  bool mayPassThrough() const;
  bool mayClimbUpFrom() const;
  bool mayWalkOnTop() const;
  const std::string& getName() const;
  bool supportsJob(PlayerRequestType type) const;
  bool isSeeThrough() const;
  bool spawnsItem() const;
  std::unique_ptr<Item> getItem() const;
  const BlockIdentifier& getIdentifier() const;

  bool isExplored() const;
  void setExplored();

 private:
  static void initialize();
  static bool mInitialized;
  bool mExplored{false};
  const BlockIdentifier mIdentifier;
};

void to_json(SerializedObj &out, const Block &block);
