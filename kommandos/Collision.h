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
	bool MeshNodeWithMeshNode(const scene::IMeshSceneNode* tBox1, const scene::IMeshSceneNode* tBox2);
};

