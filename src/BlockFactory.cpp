#include "BlockFactory.h"

#include <functional>
#include <map>
#include <memory>

#include "AirBlock.h"
#include "BlockIdentifier.h"
#include "BlockType.h"
#include "GrassBlock.h"
#include "RockBlock.h"
#include "StairUpDownBlock.h"

namespace {
template <typename T>
std::unique_ptr<Block> generateBlock() {
  return std::make_unique<T>();
}

const std::map<BlockType, std::function<std::unique_ptr<Block>()>> typeToBlock{
    {BlockTypeGrassBlock, generateBlock<GrassBlock>},
    {BlockTypeAirBlock, generateBlock<AirBlock>},
    {BlockTypeRockBlock, generateBlock<RockBlock>},
    {BlockTypeStairUpDownBlock, generateBlock<StairUpDownBlock>},

};

}  // namespace
std::unique_ptr<Block> BlockFactory::makeBlock(const BlockIdentifier& ident) {
  BlockType type = ident.getBlockType();
  ASSERT(typeToBlock.count(type), "Unknown blockidentifer");
  return typeToBlock.at(type)();
}