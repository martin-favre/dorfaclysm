#include "MiningRequest.h"

#include "Engine.h"
#include "GridMap.h"
#include "Logging.h"
MiningRequest::MiningRequest(const BlockIdentifier &blockIdent,
                             const Vector3DInt &pos, const Uuid &owningObjIdent)
    : mGridMap(GridMap::getActiveMap()),
      mTarget(blockIdent),
      mPos(pos),
      mOwningObjIdent(owningObjIdent) {}

MiningRequest::MiningRequest(const SerializedObj &serObj)
    : mGridMap(GridMap::getActiveMap()),
      mTarget(serObj.at("target")),
      mPos(serObj.at("position")),
      mOwningObjIdent(serObj.at("owner")) {}

MiningRequest::~MiningRequest() {
  GameObject *obj = Engine::getGameObject(mOwningObjIdent);
  if (obj) {
    obj->destroy();
  } else {
    LOGL("Could not destoy owning object, was not found", Logging::warning);
  }
}

const BlockIdentifier &MiningRequest::getBlockIdentifier() const {
  return mTarget;
}

bool MiningRequest::isValid() const {
  return mGridMap.blockIdentifierMatches(mTarget, mPos);
}

const Vector3DInt &MiningRequest::getPos() const { return mPos; }
bool MiningRequest::operator==(const MiningRequest &other) const {
  ASSERT(isValid(), "Check if block is valid before calling");
  ASSERT(other.isValid(), "Check if block is valid before calling");
  return mTarget == other.mTarget && mPos == other.mPos;
  }

void to_json(SerializedObj& out, const MiningRequest& vec) {
  out["target"] = vec.mTarget;
  out["position"] = vec.mPos;
  out["owner"] = vec.mOwningObjIdent;
}