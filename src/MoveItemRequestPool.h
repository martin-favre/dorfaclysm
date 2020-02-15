#pragma once
#include "GenericRequestPool.h"
#include "Item.h"

class MoveItemRequest {
 public:
  MoveItemRequest(ItemType item, const Vector3DInt& pos)
      : mItem(item), mPos(pos) {}
  bool operator==(const MoveItemRequest& other) const {
    (void)other;
    // all are unique
    return false;
  }
  Vector3DInt getPos() const { return mPos; }
  const ItemType& getType() const { return mItem; }

 private:
  const ItemType mItem;
  const Vector3DInt mPos;
};

class MoveItemRequestPool : public GenericRequestPool<MoveItemRequest> {
 public:
  static MoveItemRequestPool& getInstance();

 private:
  MoveItemRequestPool() = default;
  static MoveItemRequestPool mInstance;
};