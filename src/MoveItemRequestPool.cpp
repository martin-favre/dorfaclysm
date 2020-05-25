#include "MoveItemRequestPool.h"

MoveItemRequest::MoveItemRequest(ItemType item, const Vector3DInt& pos)
    : mItem(item), mPos(pos) {}
MoveItemRequest::MoveItemRequest(const SerializedObj& serObj)
    : mItem(serObj.at("type")), mPos(serObj.at("position")) {}

bool MoveItemRequest::operator==(const MoveItemRequest& other) const {
  (void)other;
  // all are unique
  return false;
}

void to_json(SerializedObj& out, const MoveItemRequest& req) {
  out["position"] = req.getPos();
  out["type"] = req.getType();
}

MoveItemRequestPool MoveItemRequestPool::mInstance;
MoveItemRequestPool& MoveItemRequestPool::getInstance() { return mInstance; }
