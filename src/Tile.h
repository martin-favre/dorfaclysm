
#pragma once
class Tile {
 public:
  enum TileType { notSet, grass, dirt};
  bool isOpen() const;
  bool mOpen{true};
  TileType mType{notSet};
};