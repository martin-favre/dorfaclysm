#include "BlockIdentifier.h"

#include <memory>

#include "BlockType.h"

BlockIdentifier::BlockIdentifier(const BlockType& type) : mType(type) {}
BlockIdentifier::BlockIdentifier(const SerializedObj& serObj)
    : mVersion(serObj.at("version")), mType(serObj.at("type")) {}
bool BlockIdentifier::operator==(const BlockIdentifier& block) const {
  return mVersion == block.mVersion && mType == block.mType;
}
int BlockIdentifier::getVersion() const { return mVersion; }
BlockType BlockIdentifier::getBlockType() const { return mType; }

BlockIdentifier BlockIdentifier::generateReplacement(const BlockType& newType) const {
  BlockIdentifier out{newType};
  out.mVersion = mVersion + 1;
  return out;
}

void to_json(SerializedObj& out, const BlockIdentifier& vec) {
  out["version"] = vec.getVersion();
  out["type"] = vec.getBlockType();
}