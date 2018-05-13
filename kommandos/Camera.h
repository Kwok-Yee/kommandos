#pragma once
#include <irrlicht.h>
#include "Player.h"

using namespace irr;
using namespace core;
using namespace scene;

class Camera
{
public:
	Camera(irr::IrrlichtDevice* device);
	void CameraUpdate();

private:
	 void CameraInit();

};