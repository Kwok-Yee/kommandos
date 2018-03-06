#include "Collision.h"
#include "irrlicht.h"

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

Collision::Collision()
{
}


Collision::~Collision()
{
}


bool Collision::MeshNodeWithMeshNode(const IMeshSceneNode* tBox1, const IMeshSceneNode* tBox2)
{
	
	vector3df minPos1 = tBox1->getTransformedBoundingBox().MinEdge;
	vector3df maxPos1 = tBox1->getTransformedBoundingBox().MaxEdge;
	vector3df minPos2 = tBox2->getTransformedBoundingBox().MinEdge;
	vector3df maxPos2 = tBox2->getTransformedBoundingBox().MaxEdge;
	//Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
	return(tBox1->getTransformedBoundingBox().intersectsWithBox(tBox2->getTransformedBoundingBox()) );

	//If not, it will return false

}