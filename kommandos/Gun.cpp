#include "Gun.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

IrrlichtDevice * IDevice;
ISceneNodeAnimator* anim;
vector3df endPos;
vector3df toMousePosition;
vector3df mousePosition;

Gun::Gun(ISceneNode* gun, IrrlichtDevice* device)
{
	gunNode = gun;
	IDevice = device;
	smgr = device->getSceneManager();
}

void Gun::LaserLine(vector3df endPosition, IVideoDriver * driver, ICameraSceneNode* camera)
{
	vector2di rayScreenCoordination = vector2di(endPosition.X, endPosition.Z);
	// Create a ray through the mouse cursor.
	line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(rayScreenCoordination, camera);

	// intersect the ray with a plane around the node facing towards the camera.
	plane3df plane(gunNode->getPosition(), vector3df(0, -1, 0));
	if (plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition)) {
		toMousePosition = vector3df(mousePosition - gunNode->getPosition());
		// mouse position in 3d space
	}
	driver->draw3DLine(vector3df(endPosition.X, 0, endPosition.Z), toMousePosition);
}

void Gun::Shoot(ISceneNode* bullet)
{

	if (bullet) {
		bullet->setVisible(true);
		//bullet->setMaterialTexture(0, driver->getTexture("../media/fireball.bmp"));
		hasShot = true;

		endPos = toMousePosition;

		anim = smgr->createFlyStraightAnimator
		(gunNode->getAbsolutePosition(), //spawn point
			endPos, // destination
			1000); //speed to destination
		bullet->addAnimator(anim);
		anim->drop();
	}
}

bool Gun::CheckAnimEnd(ISceneNode* bullet) 
{
	if (bullet->getPosition() == endPos) {

		hasShot = false;
		return true;
	}
	return false;
}

