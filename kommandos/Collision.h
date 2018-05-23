#pragma once
#include <irrlicht.h>

static class Collision
{
public:
	//add objects to an update list, to distinguish them
	void AddWallToList(irr::scene::ISceneNode* wall);
	void AddStaticToList(irr::scene::ISceneNode* staticObject);
	void AddDynamicToList(irr::scene::ISceneNode* dynamicObject);
	void RemoveDynamicFromList(irr::scene::ISceneNode* dynamicObject);
	//compare boundingboxes for overlap
	bool SceneNodeWithSceneNode(irr::scene::ISceneNode* tBox1, irr::scene::ISceneNode* tBox2);
	//returns true if the dynamicObject collides with a static
	bool CollidesWithStaticObjects(irr::scene::ISceneNode* dynamicObject);
	//passive update, resolves interpenetration with O(n2)
	void DiscreteCollisionUpdate();
};

