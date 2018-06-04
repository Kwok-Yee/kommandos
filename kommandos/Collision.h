#pragma once
#include <irrlicht.h>

static class Collision
{
public:
	//Checks collision between 2 IScene nodes
	bool SceneNodeWithSceneNode(irr::scene::ISceneNode* tBox1, irr::scene::ISceneNode* tBox2);

	void AddStaticToList(irr::scene::ISceneNode* staticObject);
	//returns true if the dynamicObject collides with a static
	bool CollidesWithStaticObjects(irr::scene::ISceneNode* dynamicObject);
	bool LineCollidesWithStaticObjects(irr::core::line3df line);
	irr::scene::ISceneNode* GetCollidedObjectWithLine(irr::core::line3df line);
};

