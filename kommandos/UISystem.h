#pragma once
#include <irrlicht.h>

class UISystem
{
public:

	irr::video::SColor inActiveRapidColor;
	irr::video::SColor activeRapidColor;

	void WaveUI(irr::IrrlichtDevice* device);
	void InitUISystem(irr::IrrlichtDevice* device);
	void SetRapidUIColor(irr::video::SColor color);
private:
	irr::video::SColor rapidUIColor;
};