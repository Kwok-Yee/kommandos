#include "Pathfinding.h"
#include <irrlicht.h>
#include <iostream>
#include "GridNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

Pathfinding::Pathfinding() {
	initStartGoal = false;
	foundGoal = false;
}

void Pathfinding::FindPath(vector3df currentPos, vector3df targetPos) 
{
	//if the start and the goal isn't initialized
	if (!initStartGoal) 
	{
		//delete all the list.
		for (int i = 0; i < openList.size(); i++)
		{
			delete openList[i];
		}
		openList.clear();
		for (int i = 0; i < visitedList.size(); i++)
		{
			delete visitedList[i];
		}
		visitedList.clear();
		for (int i = 0; i < pathToGoal.size(); i++)
		{
			delete pathToGoal[i];
		}
		pathToGoal.clear();

		//Initialize start and goal
		GridCell start;
		start.xCoord = currentPos.X;
		start.yCoord = currentPos.Z;

		GridCell goal;
		goal.xCoord = targetPos.X;
		goal.yCoord = targetPos.Z;

		SetStartAndGoal(start, goal);
		initStartGoal = true;
	}
	while (initStartGoal && !foundGoal) 
	{
		ContinuePath();
	}
}

void Pathfinding::SetStartAndGoal(GridCell start, GridCell goal) 
{
	startCell = new GridCell(start.xCoord, start.yCoord, NULL);
	goalCell = new GridCell(goal.xCoord, goal.yCoord, &goal);

	startCell->g = 0;
	startCell->h = startCell->ManHattanDistance(goalCell);
	startCell->parent = 0;

	openList.push_back(startCell);
}

GridCell* Pathfinding::GetNextCell() 
{
	f32 bestF = 99999.0f;
	s32 cellIndex = -1;
	GridCell* nextCell = NULL;

	for (s32 i = 0; i < openList.size(); i++)
	{
		if(openList[i]->GetF() < bestF)
		{
			bestF = openList[i]->GetF();
			cellIndex = i;
		}
	}
	
	if (cellIndex >= 0) 
	{
		nextCell = openList[cellIndex];
		visitedList.push_back(nextCell);
		openList.erase(cellIndex);
	}

	return nextCell;
}

void Pathfinding::PathOpen(s32 x, s32 y, f32 newCost, GridCell *parent) 
{
	if (x > 80 || x < -85 || y > 80 || y < -80)//boundries of the game 
	{
		return;
	}
	/*if (CELL_BLOCKED) 
	{
		return;
	}*/

	s32 id = y * WORLD_SIZE + x;
	for (int i = 0; i < visitedList.size(); i++)
	{
		if (id == visitedList[i]->id)
			return;
	}

	GridCell* newChild = new GridCell(x, y, parent);
	newChild->g = newCost;
	newChild->h = parent->ManHattanDistance(goalCell);

	for (int i = 0; i < openList.size(); i++)
	{
		if (id == openList[i]->id) 
		{
			f32 newF = newChild->g + newCost + openList[i]->h;

			if (openList[i]->GetF() > newF) 
			{
				openList[i]->g = newChild->g + newCost;
				openList[i]->parent = newChild;
			}
			else 
			{
				delete newChild;
				return;
			}
		}
	}

	openList.push_back(newChild);
}

void Pathfinding::ContinuePath()
{
	if (openList.empty())
		return;

	GridCell* currentCell = GetNextCell();

	if (currentCell->id == goalCell->id)
	{
		goalCell->parent = currentCell->parent;

		GridCell* getPath;

		for (getPath = goalCell; getPath != NULL; getPath = getPath->parent)
		{
			pathToGoal.push_back(new vector3df(getPath->xCoord, 0, getPath->yCoord));
		}

		foundGoal = true;
		return;
	}
	else
	{
		//rightCell
		PathOpen(currentCell->xCoord + 1, currentCell->yCoord, currentCell->g + 1, currentCell);
		//leftCell
		PathOpen(currentCell->xCoord - 1, currentCell->yCoord, currentCell->g + 1, currentCell);
		//topCell
		PathOpen(currentCell->xCoord, currentCell->yCoord + 1, currentCell->g + 1, currentCell);
		//bottomCell
		PathOpen(currentCell->xCoord, currentCell->yCoord - 1, currentCell->g = 1, currentCell);
		//right-topCell
		PathOpen(currentCell->xCoord + 1, currentCell->yCoord + 1, currentCell->g + 1.414, currentCell);
		//left-topCell
		PathOpen(currentCell->xCoord - 1, currentCell->yCoord + 1, currentCell->g + 1.414, currentCell);
		//right-bottomCell
		PathOpen(currentCell->xCoord + 1, currentCell->yCoord - 1, currentCell->g + 1, currentCell);
		//left-bottomCell
		PathOpen(currentCell->xCoord - 1, currentCell->yCoord - 1, currentCell->g = 1, currentCell);

		for (int i = 0; i < openList.size(); i++)
		{
			if (currentCell->id == openList[i]->id)
			{
				openList.erase(i);
			}
		}
	}
}

vector3df Pathfinding::NextPathPos(scene::ISceneNode* enemy)
{
	int index = 1;

	vector3df nextPos;
	nextPos.X = pathToGoal[pathToGoal.size() - index ]->X;
	nextPos.Z = pathToGoal[pathToGoal.size() - index ]->Z;

	vector3df distance = nextPos - enemy->getPosition();
	if (index < pathToGoal.size()) 
	{
		if (distance.getLength() < 1) 
		{
			pathToGoal.erase(pathToGoal.size() - index);
		}
	}

	return nextPos;
}



