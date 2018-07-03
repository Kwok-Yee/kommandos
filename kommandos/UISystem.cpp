#include "UISystem.h"
#include <irrlicht.h>
#include "EnemySpawner.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IGUIEnvironment* GUI_UISystem;
IGUIFont* font2_UI;

EGUI_ALIGNMENT upperleft_UI, upperright_UI;
u32 waveTimer;

void UISystem::WaveUI(IrrlichtDevice* device)
{
	GUI_UISystem = device->getGUIEnvironment();
	waveTimer = device->getTimer()->getTime();


	//Showing the current wave
	core::stringw waveNumberString = L"Current wave: ";
	waveNumberString += currentWave;
	font2_UI = device->getGUIEnvironment()->getFont("../media/Fonts/fontlucida.png");
	font2_UI->draw(stringw(waveNumberString),
		core::rect<s32>(600, 40, 200, 100),
		video::SColor(255, 255, 255, 255));

}

void UISystem::DisplayWaves()
{

}
