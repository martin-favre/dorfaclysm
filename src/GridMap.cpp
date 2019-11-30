#include "GridMap.h"
#include "Vector2DInt.h"
#include "Helpers.h"

GridMap::GridMap(const Vector2DInt& mapSize)
{
    ASSERT(mapSize.x > 0, "Must have size > 0");
    ASSERT(mapSize.y > 0, "Must have size > 0");
    for(int y = 0; y < mapSize.y; ++y)
    {
        std::vector<Tile> row;
        for(int x = 0; x < mapSize.x; ++x)
        {
            row.emplace_back();
        }
        mTiles.push_back(row);
    }
}

bool GridMap::isPosInMap(const Vector2DInt& pos) const
{
    if(pos.x < 0) return false;
    if(pos.y < 0) return false;
    if((size_t)pos.y >= mTiles.size()) return false;
    if((size_t)pos.x >= mTiles[0].size()) return false;
    return true;
}

const Tile* GridMap::getTile(const Vector2DInt& pos) const
{
    if(!isPosInMap(pos)) return nullptr;
    return &mTiles[pos.y][pos.x];
}

bool GridMap::isTileFree(const Vector2DInt& pos) const
{
    if(!isPosInMap(pos)) return false;
    return getTile(pos)->getIfTileIsEnterable();
}
