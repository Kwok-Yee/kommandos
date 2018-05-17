#pragma once
#include <irrlicht.h>
#include "GridNode.h"
class Pathfinding
{
public:
	Pathfinding();

	void FindPath(irr::core::vector3df currentPos, irr::core::vector3df targetPos);
	irr::core::vector3df NextPathPos(irr::scene::ISceneNode* enemy);
	void ClearOpenList() { openList.clear(); };
	void ClearVisitedList() { visitedList.clear(); };
	void ClearPathToGoal() { pathToGoal.clear(); };
	bool initStartGoal;
	bool foundGoal;

private:
	void SetStartAndGoal(GridCell start, GridCell goal);
	void PathOpen(irr::s32 x, irr::s32 y, irr::f32 newCost, GridCell *parent);
	GridCell *GetNextCell();
	void ContinuePath();

	GridCell *startCell;
	GridCell *goalCell;
	irr::core::array<GridCell*> openList;
	irr::core::array<GridCell*> visitedList;
	irr::core::array<irr::core::vector3df*> pathToGoal;
};


