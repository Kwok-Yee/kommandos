#include "Pathfinding.h"
#include <irrlicht.h>
#include <iostream>
#include "GridNode.h"
#include "Game.h"
#include "ObjectPlacementGeneration.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace std;

Game* pGame;
ObjectPlacementGeneration objPlace;
ISceneNode* obstaclesToAvoid[AMOUNT_OF_OBJECTS];

Pathfinding::Pathfinding() {

	initStartGoal = false;
	foundGoal = false;
	pGame = pGame->GetInstance();
	objPlace = pGame->objectPlacementGen;
	for (int i = 0; i < AMOUNT_OF_OBJECTS; i++)
	{
		obstaclesToAvoid[i] = pGame->objectPlacementGen.obstacles[i];//gets the objects from the objecPlacementGen script
	}
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
		if(openList[i]->GetF() < bestF)//Finding the cheapest cell in this list
		{
			bestF = openList[i]->GetF();
			cellIndex = i;
		}
	}
	
	if (cellIndex >= 0)
	{
		//Add the cheapest cell in openList to visitedList
		nextCell = openList[cellIndex];
		visitedList.push_back(nextCell);
		openList.erase(cellIndex);
	}

	return nextCell;//Returns the cheapest cell
}

void Pathfinding::PathOpen(s32 x, s32 y, f32 newCost, GridCell *parent) 
{
	if (x > 79 || x < -79 || y > 79 || y < -79)//boundries of the game 
	{
		return;
	}
	for (int i = 0; i < AMOUNT_OF_OBJECTS; i++)
	{//Checks if there's a obstacle on this cell
		if (x == objPlace.obstacles[i]->getPosition().X && y == objPlace.obstacles[i]->getPosition().Z)
		{
			//cout << "object avoided at: " << x << " , " << y << endl;
			return;
		}
	}

	s32 id = x * WORLD_SIZE + y;
	for (int i = 0; i < visitedList.size(); i++)//Checks if this list is visited via id
	{
		if (id == visitedList[i]->id)
			return;
	}
	
	GridCell* newChild = new GridCell(x, y, parent);
	newChild->g = newCost;
	newChild->h = parent->ManHattanDistance(goalCell);

	for (int i = 0; i < openList.size(); i++)
	{
		if (id == openList[i]->id) //Checks if this cell is already in the openList
		{
			f32 newF = newChild->g + newCost + openList[i]->h;

			if (openList[i]->GetF() > newF) //If there is one then update the cost of that cell
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

	if (currentCell->id == goalCell->id)//Checks if the goal is reached
	{
		goalCell->parent = currentCell->parent;//If it reached the goalcell set the goalcell's parent to currentcell's parent

		GridCell* getPath;//Array of the path in cells.

		for (getPath = goalCell; getPath != NULL; getPath = getPath->parent)//If the goal is reached then fill the pathToGoal list
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
		PathOpen(currentCell->xCoord + 1, currentCell->yCoord - 1, currentCell->g + 1.414, currentCell);
		//left-bottomCell
		PathOpen(currentCell->xCoord - 1, currentCell->yCoord - 1, currentCell->g + 1.414, currentCell);

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
		{//if he is almost at the position, delete it, so next time it will take the next position in the list
			pathToGoal.erase(pathToGoal.size() - index);
		}
	}
	return nextPos;
}



