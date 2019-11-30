#include <limits>
#include "Astar.h"
#include "Helpers.h"
#include <queue>
#include <map>
#include "GridMap.h"
#include "Logging.h"

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
	static constexpr int level_weight = 40;
#if (DIRECTIONS == 8)
	static constexpr int diag_level_wight = 80;
#endif
	static constexpr int estimation_weight = 10;


	Vector2DInt position;
	// total distance already traveled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority
	int direction;

	
	const AStarNode * parent;
public:
	std::string name;
	AStarNode(const Vector2DInt & vec, int d, int p, int dir, AStarNode  * par) :
		position(vec), level(d), priority(p), direction(dir), parent(par) {}
	~AStarNode(){}
	const Vector2DInt & get_pos() const { return position; }
	int getLevel() const { return level; }
	int getPriority() const { return priority; }
	int getDir()const { return direction; }
	const AStarNode * get_parent() const { return parent; }

	void updatePriority(const Vector2DInt & goal)
	{
		priority = level + estimate(goal)*estimation_weight; 
	}

	void nextLevel(const int & direction) 
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
		const int d = Helpers::roundToInt(Helpers::getSquaredPositionBetweenPositions(goal, position));

		// const int xd = goal.x - position.x;
		// const int yd = goal.x - position.y;
		// Manhattan distance
		// const int d=abs(xd)+abs(yd);

		// Chebyshev distance
		// const int d=std::max(abs(xd), abs(yd));

		return(d);
	}
};

class Compare
{
public:
	bool operator() (const AStarNode* a, const AStarNode* b)
	{
		return a->getPriority() > b->getPriority();
	}
};

bool Astar::getPath(const Vector2DInt& start, const Vector2DInt& finish, const GridMap& map, std::stack<Vector2DInt> & path)
{
	if(!map.isPosInMap(start))
	{
		Logging::log("Could not find path, the start was outside map");
		return false;
	}
	if(!map.isPosInMap(finish))
	{
		Logging::log("Could not find path, the target position outside map");
		return false;
	} 
	if (!map.isTileFree(finish))
	{
		Logging::log("Could not find path, the target position was occupied");
		return false;
	}
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, Compare> node_queue;
	AStarNode * current_node = new AStarNode(start, 0, 0, 0, nullptr);
	current_node->updatePriority(finish);
	node_queue.push(current_node);
	std::map<Vector2DInt, int> map_weights;
	map_weights[start] = current_node->getPriority();
	int steps = 0;
	std::vector<AStarNode*> old_nodes;
	while (!node_queue.empty())
	{
		++steps;
		if (steps > 10000)
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
					map.isPosInMap(new_pos) &&
					map.isTileFree(new_pos);
				if (valid_step)
				{
					AStarNode * next_node = new AStarNode(new_pos, current_node->getLevel(), current_node->getPriority(), dir, current_node);
					next_node->nextLevel(dir);
					next_node->updatePriority(finish);

					/* if my priority is better, mark it down and continue */
					if (map_weights.count(new_pos) == 0)
					{
						map_weights[new_pos] = std::numeric_limits<int>::max();
					}

					if (next_node->getPriority() < map_weights[new_pos])
					{
						map_weights[new_pos] = next_node->getPriority();
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

