#include <limits>
#include "Astar.h"
#include "Helpers.h"
#include <fstream>
#include <iostream>
#include <iomanip>

/*
	Implementation stolen/inspired from here:
	http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
*/

#define DIRECTIONS 4 // number of possible directions to go at any position

#if (DIRECTIONS == 4)
constexpr int DX[DIRECTIONS]={1, 0, -1, 0};
constexpr int DY[DIRECTIONS]={0, 1, 0, -1};
#else
constexpr int DX[DIRECTIONS] = { 1, 1, 0, -1, -1, -1, 0, 1 };
constexpr int DY[DIRECTIONS] = { 0, 1, 1, 1, 0, -1, -1, -1 };
#endif



class AStarNode
{
	/* Tuning parameters 
		Increase level weights to punish every extra level taken
		Increase estimation weight to punish the distance estimation.
	*/
	constexpr int level_weight = 40;
#if (DIRECTIONS == 8)
	constexpr int diag_level_wight = 80;
#endif
	constexpr int estimation_weight = 10;


	Vector2DInt position;
	// total distance already traveled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority
	int direction;

	
	AStarNode const * const parent;
public:
	std::string name;
	AStarNode(const Vector2DInt & vec, int d, int p, int dir, AStarNode  * par) :
		position(vec), level(d), priority(p), direction(dir), parent(par) {}
	~AStarNode(){}
	const Vector2DInt & get_pos() const { return position; }
	int get_level() const { return level; }
	int get_priority() const { return priority; }
	int get_dir()const { return direction; }
	AStarNode const * const get_parent() const { return parent; }

	void update_priority(const Vector2DInt & goal)
	{
		priority = level + estimate(goal)*estimation_weight; 
	}

	void next_level(const int & direction) 
	{
	// give better priority to going straight instead of diagonally
#if (DIRECTIONS == 4)
		(void)direction;
		level += level_weight;
#else
		level += (direction % 2 == 0 ? level_weight : diag_level_wight);
#endif
	}

	// Estimation function for the remaining distance to the goal.
	int estimate(const Vector2DInt & goal) const
	{
		// Squared Euclidean Distance
		const int d = helpers::round_to_int(helpers::get_squared_distance_between_positions(goal, position));

		//const int xd = goal.x - position.x;
		//const int yd = goal.x - position.y;
		// Manhattan distance
		//const int d=abs(xd)+abs(yd);

		// Chebyshev distance
		//const int d=max(abs(xd), abs(yd));

		return(d);
	}
};

class Compare
{
public:
	bool operator() (AStarNode const * const a, AStarNode const * const b)
	{
		return a->get_priority() > b->get_priority();
	}
};

bool Astar::get_path(Vector2DInt start, Vector2DInt finish, const GridMap& map, std::stack<Vector2DInt> & path)
{
	if(!map.get_if_position_in_gridmap(start)) return false;
	if(!map.get_if_position_in_gridmap(finish)) return false;
	if (!map.get_if_tile_is_free(finish))
	{
		Logging::log("Could not find path, the target position was not eligible");
		return false;
	}
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, Compare> node_queue;
	AStarNode * current_node = new AStarNode(start, 0, 0, 0, nullptr);
	current_node->update_priority(finish);
	node_queue.push(current_node);
	std::map<Vector2DInt, int> map_weights;
	map_weights[start] = current_node->get_priority();
	int steps = 0;
	std::vector<AStarNode*> old_nodes;
	while (!node_queue.empty())
	{
		++steps;
		if (steps > 1000)
		{
			Logging::log("Could not find path, over max number of steps");
			return false;
		}
		current_node = node_queue.top();
		node_queue.pop();
		
		if (current_node->get_pos() == finish )
		{
			AStarNode const * curr = current_node;
			while (curr->get_parent() != nullptr)
			{
				Vector2DInt pos = curr->get_pos();
				pos = map.gridspace_to_pixelspace(pos);
				path.push(pos);
				curr = curr->get_parent();
			}
			while (!node_queue.empty())
			{
				delete node_queue.top();
				node_queue.pop();
			}
			for (auto it = old_nodes.begin(); it != old_nodes.end(); ++it)
			{
				delete *it;
			}
			delete current_node;
			return true;
			/* Generate output path and cleanup*/
		}
		else
		{
			for (int dir = 0; dir < DIRECTIONS; ++dir)
			{
				const Vector2DInt current_pos = current_node->get_pos();
				const Vector2DInt new_pos(current_pos.x + DX[dir], current_pos.y + DY[dir]);
				
				/*if x and y are within boundaries and 
				the tile is not impassable and
				then it's a valid step
				*/
				const bool valid_step =
					map.get_if_position_in_gridmap(new_pos) &&
					map.get_if_tile_is_free(new_pos);
				if (valid_step)
				{
					AStarNode * const next_node = new AStarNode(new_pos, current_node->get_level(), current_node->get_priority(), dir, current_node);
					next_node->next_level(dir);
					next_node->update_priority(finish);

					/* if my priority is better, mark it down and continue */
					if (map_weights.count(new_pos) == 0)
					{
						map_weights[new_pos] = std::numeric_limits<int>::max();
					}

					if (next_node->get_priority() < map_weights[new_pos])
					{
						map_weights[new_pos] = next_node->get_priority();
						node_queue.push(next_node);
					}
					else
					{
						delete next_node;
					}
				}
			}
			old_nodes.push_back(current_node);
		}
	}
	Logging::log("Could not find path, no possible path");
	return false;
}

