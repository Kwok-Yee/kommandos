#pragma once
#include <irrlicht.h>


static class Collision
{
public:

	Collision();
	~Collision();
	//Checks collision between 2 IScene nodes
	bool SceneNodeWithSceneNode(ISceneNode* tBox1, ISceneNode* tBox2);
	void AddStaticToList(ISceneNode* staticObject);
	void RemoveStaticFromList(ISceneNode* staticObject);
	bool CollidesWithStaticObjects(ISceneNode* dynamicObject);
};

