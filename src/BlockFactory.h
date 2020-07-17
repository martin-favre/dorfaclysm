#pragma once
#include <algorithm>
#include <memory>

class Block;
class BlockIdentifier;

class BlockFactory {
  public:

  static std::unique_ptr<Block> makeBlock(const BlockIdentifier& ident);

};