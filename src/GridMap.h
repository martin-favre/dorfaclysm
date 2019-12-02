
#include <vector>
#include "Tile.h"

class Vector2DInt;
class GridMap
{
    public:
    GridMap(const Vector2DInt& mapSize);
    bool isPosInMap(const Vector2DInt& pos) const;
    bool isTileFree(const Vector2DInt& pos) const;
    const Tile* getTile(const Vector2DInt& pos) const;
    bool setTile(const Vector2DInt& pos, const Tile& tile);
    private:
    std::vector<std::vector<Tile>> mTiles;
};