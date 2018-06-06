#include "Collision.h"
#include "irrlicht.h"
#include <cmath>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif	

core::array<ISceneNode*> staticList;

bool Collision::SceneNodeWithSceneNode(ISceneNode* tBox1, ISceneNode* tBox2)
{
	return(tBox1->getTransformedBoundingBox().intersectsWithBox(tBox2->getTransformedBoundingBox()));
}

void Collision::AddStaticToList(ISceneNode* staticObject)
{
	staticList.push_back(staticObject);
}

bool Collision::CollidesWithStaticObjects(ISceneNode* dynamicObject)
{
	for (u32 i = 0; i < staticList.size(); i++)
		if (SceneNodeWithSceneNode(dynamicObject, staticList[i]))
			return true;
	return false;
}

bool Collision::LineCollidesWithStaticObjects(line3df line)
{
	for (u32 i = 0; i < staticList.size(); i++)
		if (staticList[i]->getBoundingBox().intersectsWithLine(line))
			return true;
	return false;
}

ISceneNode* Collision::GetCollidedObjectWithLine(line3df line)
{
	for (u32 i = 0; i < staticList.size(); i++) {
		if (staticList[i]->getBoundingBox().intersectsWithLine(line))
			return staticList[i];
	}
	return false;
}

//a^2 + b^2 = c^2
f32 Collision::distance(vector3df a, vector3df b)
{
	return sqrt((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y));
}

bool Collision::lineIntersectsCircle(vector3df ahead, vector3df ahead2, ISceneNode* obstacle)
{
	// the property "center" of the obstacle is a Vector3D.
	return distance(obstacle->getPosition(), ahead) <= 1 || distance(obstacle->getPosition(), ahead2) <= 1;
}

//finds object which may have a collision with
ISceneNode* Collision::findMostThreateningObstacle(vector3df ahead, vector3df position)
{
	ISceneNode* mostThreatening = NULL;

	for (int i = 0; i < staticList.size(); i++) {
		ISceneNode* obstacle = staticList[i];
		bool collision = lineIntersectsCircle(ahead, ahead / 2, obstacle);

		// "position" is the character's current position
		if (collision && (mostThreatening == NULL || distance(position, obstacle->getPosition()) < distance(position, mostThreatening->getPosition()))) {
			mostThreatening = obstacle;
		}
	}
	return mostThreatening;
}

