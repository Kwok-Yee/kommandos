#include "UISystem.h"
#include <irrlicht.h>

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IGUIEnvironment* GUI_UISystem;
IGUIFont* font2_UI;
u32 waveTimer;

EGUI_ALIGNMENT upperleft_UI, upperright_UI;

s32 currentWave = 0;
//IGUIStaticText* scoreText;

void UISystem::UI(IrrlichtDevice* device)
{
	GUI_UISystem = device->getGUIEnvironment();
	waveTimer = device->getTimer()->getTime();

	font2_UI = device->getGUIEnvironment()->getFont("../media/Fonts/fontlucida.png");
	font2_UI->draw(stringw("Current wave: " + currentWave).c_str(),
		core::rect<s32>(600, 40, 200, 100),
		video::SColor(255, 255, 255, 255));

	DisplayWaves(/*0*/);
}

//updates the score of the player 
void UISystem::DisplayWaves(/*f32 points*/)
{

	//score += points;

}
