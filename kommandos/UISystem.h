#pragma once
#include <irrlicht.h>

class UISystem
{
public:
	UISystem(irr::IrrlichtDevice* device);
	void InitUISystem(irr::IrrlichtDevice* device);
	void WaveUI(irr::IrrlichtDevice* device);
};