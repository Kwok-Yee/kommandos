#pragma once
#include <irrlicht.h>

static class Collision
{
public:
	//Checks collision between 2 IScene nodes
	bool SceneNodeWithSceneNode(irr::scene::ISceneNode* tBox1, irr::scene::ISceneNode* tBox2);
	void AddStaticToList(irr::scene::ISceneNode* staticObject);
	bool CollidesWithStaticObjects(irr::scene::ISceneNode* dynamicObject);
};

