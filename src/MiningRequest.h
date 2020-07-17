#pragma once
#include <functional>
#include <memory>

#include "Block.h"
#include "BlockIdentifier.h"
#include "Serializer.h"
#include "Uuid.h"
#include "Vector3DInt.h"

class GridMap;
class MiningRequest {
 public:
  MiningRequest(const SerializedObj& serObj);
  MiningRequest(const BlockIdentifier& blockIdent, const Vector3DInt& pos,
                const Uuid& owningObjIdent);
  MiningRequest(const MiningRequest&) = delete;
  MiningRequest(const MiningRequest&&) = delete;
  const MiningRequest& operator=(const MiningRequest&) = delete;
  const MiningRequest& operator=(const MiningRequest&&) = delete;
  ~MiningRequest();
  bool isValid() const;
  const Vector3DInt& getPos() const;
  bool operator==(const MiningRequest& other) const;
  const BlockIdentifier& getBlockIdentifier() const;

  friend void to_json(SerializedObj& out, const MiningRequest& vec);

 private:
  const GridMap& mGridMap;
  
  const BlockIdentifier mTarget;
  const Vector3DInt mPos;
  const Uuid mOwningObjIdent;
};

void to_json(SerializedObj& out, const MiningRequest& vec);