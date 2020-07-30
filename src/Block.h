#pragma once
#include <memory>

#include "BlockIdentifier.h"
#include "BlockType.h"
#include "ItemType.h"
#include "PlayerRequestType.h"
#include "Serializer.h"

class GridActor;
class Item;
class Sprite;

class Block {
 public:
  Block();
  Block(BlockType);
  Block(const BlockIdentifier& identifier);
  Block(const Block&) = default;
  Block(Block&&) = default;
  Block(const SerializedObj& block);
  Block& operator=(const Block&) = default;
  Block& operator=(Block&&) = default;
  ~Block() = default;
  const Sprite* getTopSprite() const;
  bool mayPassThrough() const;
  bool mayClimbUpFrom() const;
  bool mayWalkOnTop() const;
  const std::string& getName() const;
  bool supportsJob(PlayerRequestType type) const;
  bool isSeeThrough() const;
  bool spawnsItem() const;
  ItemType getItem() const;
  const BlockIdentifier& getIdentifier() const;

  bool isExplored() const;
  void setExplored();
  friend void from_json(const SerializedObj& in, Block& block);

 private:
  static void initialize();
  static bool mInitialized;
  bool mExplored{false};
  BlockIdentifier mIdentifier{};
};

void to_json(SerializedObj& out, const Block& block);
void from_json(const SerializedObj& in, Block& block);