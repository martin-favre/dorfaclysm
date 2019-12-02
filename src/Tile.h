
#pragma once
class Tile {
 public:
  virtual ~Tile() = default;
  bool isOpen() const;

 protected:
  bool mOpen{true};
};