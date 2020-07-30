#pragma once
#include <cstddef>

#include "BlockType.h"
#include "Vector3DInt.h"

class BlockIdentifier {
 public:
  BlockIdentifier() = default;
  BlockIdentifier(const BlockType& type);
  BlockIdentifier(const SerializedObj& serObj);
  BlockIdentifier(const BlockIdentifier&) = default;
  BlockIdentifier(BlockIdentifier&&) = default;
  BlockIdentifier& operator=(BlockIdentifier&&) = default;
  BlockIdentifier& operator=(const BlockIdentifier&) = default;
  bool operator==(const BlockIdentifier& block) const;
  int getVersion() const;
  BlockType getBlockType() const;
  BlockIdentifier generateReplacement(const BlockType& newType) const;
  friend void from_json(const SerializedObj& in, BlockIdentifier& ident);

 private:
  int mVersion{0};
  BlockType mType{BlockTypeInvalid};
};

void to_json(SerializedObj& out, const BlockIdentifier& vec);
void from_json(const SerializedObj& in, BlockIdentifier& ident);