#pragma once
#include <irrlicht.h>
#include "GridNode.h"
class Pathfinding
{
public:
	Pathfinding();
	//Finds a path from currentPos to the targetPos
	void FindPath(irr::core::vector3df currentPos, irr::core::vector3df targetPos);
	/*Returns the position of the next position in the path, 
	needs to get the enemy because it will check if it reached that position
	if he did then delete that position from the list and returns the next one */
	irr::core::vector3df NextPathPos(irr::scene::ISceneNode* enemy);
	//Clears the open list
	void ClearOpenList() { openList.clear(); };
	//Clears the visited list
	void ClearVisitedList() { visitedList.clear(); };
	//Clears the path
	void ClearPathToGoal() { pathToGoal.clear(); };
	//A check if you initialized a start en goal position
	bool initStartGoal;
	//A check if the script found a path
	bool foundGoal;

private:
	//Initializes the start and goal cell
	void SetStartAndGoal(GridCell start, GridCell goal);
	//Checks the next position the the path, in recursion.
	void PathOpen(irr::s32 x, irr::s32 y, irr::f32 newCost, GridCell *parent);
	//Returns the next available cell.
	GridCell *GetNextCell();
	void ContinuePath();

	GridCell *startCell;
	GridCell *goalCell;
	irr::core::array<GridCell*> openList;//This list contains the cells it has checked.
	irr::core::array<GridCell*> visitedList;//This list contains the cells the possible is the route.
	irr::core::array<irr::core::vector3df*> pathToGoal;//list of position from startcell to the goalcell.
};


