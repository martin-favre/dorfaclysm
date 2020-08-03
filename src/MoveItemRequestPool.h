#pragma once
#include <memory>

#include "GenericRequestPool.h"
#include "Item.h"
#include "ItemType.h"
#include "Serializer.h"
#include "Uuid.h"

// Request to find an item a specific type and move it to
// the designated position
class MoveItemRequest {
 public:
  MoveItemRequest(ItemType item, const Vector3DInt& pos);
  MoveItemRequest(ItemType item, const Vector3DInt& pos,
                  std::unique_ptr<Uuid>&& recepient);
  MoveItemRequest(const SerializedObj& serObj);

  bool operator==(const MoveItemRequest& other) const;

  Vector3DInt getPos() const { return mPos; }
  const ItemType& getType() const { return mItem; }
  bool hasRecepient() const { return mRecepientUuid.get(); }
  const Uuid& getRecepient() const { return *mRecepientUuid; }

 private:
  const ItemType mItem;
  const Vector3DInt mPos;
  const std::unique_ptr<Uuid> mRecepientUuid{};
};

void to_json(SerializedObj& out, const MoveItemRequest& req);

class MoveItemRequestPool : public GenericRequestPool<MoveItemRequest> {
 public:
  static MoveItemRequestPool& getInstance();

 private:
  MoveItemRequestPool() = default;
  static MoveItemRequestPool mInstance;
};