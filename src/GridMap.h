
#include <vector>
#include "Tile.h"

class Vector2DInt;
class GridMap
{
    public:
    GridMap(const Vector2DInt& mapSize);
    bool isPosinTileMap(const Vector2DInt& pos) const;
    bool getIfTileIsFree(const Vector2DInt& pos) const;
    private:
    std::vector<std::vector<Tile>> mTiles;
};