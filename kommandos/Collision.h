#pragma once
#include <irrlicht.h>

class Collision
{
public:

	Collision();
	~Collision();
	//Checks collision between 2 IScene nodes
	bool SceneNodeWithSceneNode(const irr::scene::ISceneNode* tBox1, const irr::scene::ISceneNode* tBox2);
};

