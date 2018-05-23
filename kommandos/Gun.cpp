#include <irrlicht.h>
#include "Gun.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

IrrlichtDevice* gunIDevice;
ISceneManager* gunSmgr;
ISceneNodeAnimator* anim;
vector3df endPos;
vector3df toMousePosition;

Gun::Gun(ISceneNode* gun, IrrlichtDevice* device)
{
	gunNode = gun;
	gunIDevice = device;
	gunSmgr = device->getSceneManager();
}

void Gun::LaserLine(vector3df endPosition, IVideoDriver * driver, ICameraSceneNode* camera)
{
	float lineStartY = 0;

	RayFromScreen(endPosition, camera);
	driver->draw3DLine(vector3df(endPosition.X, lineStartY, endPosition.Z), toMousePosition);
}

void Gun::Shoot(ISceneNode* bullet)
{
	Bullet(bullet);
}

bool Gun::CheckAnimEnd(ISceneNode* bullet) 
{
	return BulletShootAnim(bullet);
}

// Raycast from screen with a start to end position.
void Gun::RayFromScreen(irr::core::vector3df &endPosition, irr::scene::ICameraSceneNode * camera)
{
	vector3df planeNormal = vector3df(0, -1, 0);
	vector2di rayScreenCoordination = vector2di(endPosition.X, endPosition.Z);
	// Create a ray through the screen coordinates.
	line3df ray = gunSmgr->getSceneCollisionManager()->getRayFromScreenCoordinates(rayScreenCoordination, camera);

	plane3df plane(gunNode->getPosition(), planeNormal);
	if (OnLineIntersect(plane, ray)) {
		toMousePosition = vector3df(mousePosition - gunNode->getPosition());
	}
}

// intersect the ray with a plane around the node facing towards the mouse.
bool Gun::OnLineIntersect(irr::core::plane3df &plane, irr::core::line3df &ray)
{
	return plane.getIntersectionWithLine(ray.start, ray.getVector(), mousePosition);
}

// bullet checks
void Gun::Bullet(irr::scene::ISceneNode * bullet)
{
	float animSpeed = 1000;
	if (bullet) {
		bullet->setVisible(true);
		hasShot = true;
		endPos = toMousePosition;

		BulletAnimation(animSpeed, bullet);
	}
}

void Gun::BulletAnimation(float animSpeed, irr::scene::ISceneNode * bullet)
{
	anim = gunSmgr->createFlyStraightAnimator
	(gunNode->getAbsolutePosition(), //spawn point
		endPos,						 // destination
		animSpeed);					 //speed to destination
	bullet->addAnimator(anim);
	anim->drop();
}

bool Gun::BulletShootAnim(irr::scene::ISceneNode * bullet)
{
	if (bullet->getPosition() == endPos) {
		hasShot = false;
		return true;
	}
	return false;
}

vector3df Gun::GetMousePosition()
{
	return mousePosition;
}