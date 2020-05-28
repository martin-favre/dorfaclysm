#include "BlockIdentifier.h"

BlockIdentifier::BlockIdentifier(const Block& block) : mType(block.getType()) {}
BlockIdentifier::BlockIdentifier(const SerializedObj& serObj)
    : mVersion(serObj.at("version")), mType(serObj.at("type")) {}
bool BlockIdentifier::operator==(const BlockIdentifier& block) const {
  return mVersion == block.mVersion && mType == block.mType;
}
int BlockIdentifier::getVersion() const { return mVersion; }
BlockType BlockIdentifier::getBlockType() const { return mType; }

void to_json(SerializedObj& out, const BlockIdentifier& vec) {
  out["version"] = vec.getVersion();
  out["type"] = vec.getBlockType();
}