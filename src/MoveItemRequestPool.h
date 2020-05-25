#pragma once
#include "GenericRequestPool.h"
#include "Item.h"
#include "Serializer.h"

class MoveItemRequest {
 public:
  MoveItemRequest(ItemType item, const Vector3DInt& pos);
  MoveItemRequest(const SerializedObj& serObj);

  bool operator==(const MoveItemRequest& other) const;

  Vector3DInt getPos() const { return mPos; }
  const ItemType& getType() const { return mItem; }

 private:
  const ItemType mItem;
  const Vector3DInt mPos;
};

void to_json(SerializedObj& out, const MoveItemRequest& req);

class MoveItemRequestPool : public GenericRequestPool<MoveItemRequest> {
 public:
  static MoveItemRequestPool& getInstance();

 private:
  MoveItemRequestPool() = default;
  static MoveItemRequestPool mInstance;
};