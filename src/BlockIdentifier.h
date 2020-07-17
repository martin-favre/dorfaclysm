#pragma once
#include <cstddef>

#include "BlockType.h"
#include "Vector3DInt.h"

class BlockIdentifier {
 public:
  BlockIdentifier(const BlockType& type);        // create new
  BlockIdentifier(const SerializedObj& serObj);  // unserialize existing
  BlockIdentifier(const BlockIdentifier&) = default;
  bool operator==(const BlockIdentifier& block) const;
  int getVersion() const;
  BlockType getBlockType() const;
  BlockIdentifier generateReplacement(const BlockType& newType)const;

 private:
  int mVersion{0};
  BlockType mType;
};

void to_json(SerializedObj& out, const BlockIdentifier& vec);