#include "GridMap.h"
#include "Vector2DInt.h"

GridMap::GridMap(const Vector2DInt& mapSize)
{
    for(int y = 0; y < mapSize.y; ++y)
    {
        for(int x = 0; x < mapSize.x; ++x)
        {
            mTiles.emplace_back();
        }
    }
}

bool GridMap::isPosinTileMap(const Vector2DInt& pos) const
{

}

bool GridMap::getIfTileIsFree(const Vector2DInt& pos) const
{

}
