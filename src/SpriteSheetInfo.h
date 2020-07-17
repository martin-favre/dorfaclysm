#pragma once
#include <string>
#include "Vector2DInt.h"
#include "Serializer.h"

class SpriteSheetInfo {
 public:
  std::string mPath;
  Vector2DInt mTileSize;
};

inline void to_json(SerializedObj& j, const SpriteSheetInfo& vec) {
  j = {{"path", vec.mPath}, {"tileSize", vec.mTileSize}};
}

inline void from_json(const SerializedObj& j, SpriteSheetInfo& vec) {
  j.at("path").get_to(vec.mPath);
  j.at("tileSize").get_to(vec.mTileSize);
}
