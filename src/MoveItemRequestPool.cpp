#include "MoveItemRequestPool.h"

#include <memory>

#include "Uuid.h"

MoveItemRequest::MoveItemRequest(ItemType item, const Vector3DInt& pos)
    : mItem(item), mPos(pos) {}
MoveItemRequest::MoveItemRequest(const SerializedObj& serObj)
    : mItem(serObj.at("type")),
      mPos(serObj.at("position")),
      mRecepientUuid(serObj.contains("recepient")
                         ? std::make_unique<Uuid>(serObj["recepient"])
                         : nullptr) {}
MoveItemRequest::MoveItemRequest(ItemType item, const Vector3DInt& pos,
                                 std::unique_ptr<Uuid>&& recepient)

    : mItem(item), mPos(pos), mRecepientUuid(std::move(recepient)) {}

bool MoveItemRequest::operator==(const MoveItemRequest& other) const {
  (void)other;
  // all are unique
  return false;
}

void to_json(SerializedObj& out, const MoveItemRequest& req) {
  out["position"] = req.getPos();
  out["type"] = req.getType();
  if (req.hasRecepient()) {
    out["recepient"] = req.getRecepient();
  }
}

MoveItemRequestPool MoveItemRequestPool::mInstance;
MoveItemRequestPool& MoveItemRequestPool::getInstance() { return mInstance; }
