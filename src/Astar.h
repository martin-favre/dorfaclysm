#pragma once
#include "basics/vector2dint.hpp"
#include <stack>
class TileInfo;
class Astar
{
public:

	/*------------------------------------
	Get path in the grid from a position, to a position.
	--------------------------------------
	Returns:   bool, if it was possible to find a path
	Parameter(out): std::stack<Vector2DInt> & path, will be populated with the path
		Not changed if no good path is found.
	------------------------------------*/
	static bool get_path(Vector2DInt from, Vector2DInt to, std::stack<Vector2DInt> & path);
	private:
	Astar() = default;
};