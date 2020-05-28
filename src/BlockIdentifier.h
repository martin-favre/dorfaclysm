#pragma once
#include "Block.h"
#include "BlockType.h"
#include "Vector3DInt.h"

class BlockIdentifier {
 public:
  BlockIdentifier(const Block& block);          // create new
  BlockIdentifier(const SerializedObj& serObj);  // unserialize existing
  BlockIdentifier(const BlockIdentifier&) = default;
  bool operator==(const BlockIdentifier& block) const;

  int getVersion() const;
  BlockType getBlockType() const;

 private:
  int mVersion{0};
  BlockType mType;
};

void to_json(SerializedObj& out, const BlockIdentifier& vec);