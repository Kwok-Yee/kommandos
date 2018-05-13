#include "Camera.h"
#include <irrlicht.h>
#include <iostream>
#include "Player.h"

using namespace irr;
using namespace core;
using namespace scene;

const vector3df CAMERASTARTPOSITION = vector3df(0, 120, 0);
const vector3df CAMERASTARTTARGET = vector3df(0, 0, 0);

IrrlichtDevice* CameraIDevice;
ISceneManager* cameraSmgr;
ICameraSceneNode* camera = cameraSmgr->addCameraSceneNode();
Player* player_; //Maybe not necessary

vector3df newCameraPosition;

Camera::Camera(IrrlichtDevice* device)
{

	CameraIDevice = device;
	//ICameraSceneNode* camera = cameraSmgr->addCameraSceneNode();
	//playerDriver = playerIDevice->getVideoDriver();
	cameraSmgr = CameraIDevice->getSceneManager();
	CameraInit();

}

void Camera::CameraInit()
{
	if (camera) 
	{
		camera->setPosition(CAMERASTARTPOSITION);
		camera->setTarget(CAMERASTARTTARGET);
	}
}

void Camera::CameraUpdate()
{
	if (camera)
	{
		newCameraPosition.Y = 80;
		newCameraPosition.X = player_->getPlayerObject()->getPosition().X;
		newCameraPosition.Z = player_->getPlayerObject()->getPosition().Z;
		camera->setPosition(newCameraPosition);
		camera->setTarget(player_->getPlayerObject()->getPosition());

	}
}