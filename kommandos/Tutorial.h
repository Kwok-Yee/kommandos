#include <irrlicht.h>

#pragma once
class Tutorial
{
public:
	void ShowTutorial(irr::IrrlichtDevice* device);
	void RemoveTutorial();
	void Update(irr::f32 frameDeltaTime);
};

