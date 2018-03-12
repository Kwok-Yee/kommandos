#pragma once
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
class Collision
{
public:

	Collision();
	~Collision();
	//Checks collision between 2 IScene nodes
	bool SceneNodeWithSceneNode(const scene::ISceneNode* tBox1, const scene::ISceneNode* tBox2);
};

