#pragma once
#include "Block.h"
#include "BlockType.h"
#include "Vector3DInt.h"

class BlockIdentifier {
 public:
  BlockIdentifier(const Block& name);          // create new
  BlockIdentifier(const SerializedObj& name);  // unserialize existing
  BlockIdentifier(const BlockIdentifier&);
  bool operator==(const BlockIdentifier& block) const;
  SerializedObj serialize() const;
  int getVersion() const;
  BlockType getBlockType() const;

  static BlockIdentifier generateInvalid();

 private:
  int mVersion;
  BlockType mType;
};

void to_json(SerializedObj& out, const BlockIdentifier& vec);