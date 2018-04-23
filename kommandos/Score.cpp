#include "Score.h"
#include <irrlicht.h>

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IrrlichtDevice* scoreIDevice;
IGUIEnvironment* scoreGui;
IVideoDriver* scoreDriver;
ISceneManager* scoreSmgr;

s32 score = 10;

Score::Score()
{
}

void Score::Scoring(IrrlichtDevice* device) {
	scoreIDevice = device;
	scoreDriver = device->getVideoDriver();
	scoreGui = device->getGUIEnvironment();
	scoreSmgr = device->getSceneManager();

	/*score += points;
		scoreGui->addStaticText(core::stringw(score).c_str(),
			rect<s32>(80, 80, 200, 100), true);*/
	DisplayScore(0);

}

void Score::DisplayScore(f32 points) {

	score += points;
	scoreGui->addStaticText(core::stringw(score).c_str(),
		rect<s32>(80, 80, 200, 100), true);
	
	
}
