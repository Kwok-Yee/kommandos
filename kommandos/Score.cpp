#include "Score.h"
#include <irrlicht.h>

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IGUIEnvironment* scoreGui;
IGUIFont* font2;
u32 timer;

EGUI_ALIGNMENT upperleft,upperright;


s32 score = 0;
IGUIStaticText* scoreText;


void Score::Scoring(IrrlichtDevice* device) 
{
	scoreGui = device->getGUIEnvironment();

	//scoreText = scoreGui->addStaticText(stringw(score).c_str(),
	//		rect<s32>(80, 80, 200, 100), true);

	timer = device->getTimer()->getTime();

	font2 = device->getGUIEnvironment()->getFont("../media/fontlucida.png");
	font2->draw(stringw(score).c_str(),
		core::rect<s32>(80, 80, 200, 100),
		video::SColor(255, 255, 255, 255));
	
	//core::rect<s32>(80, 50, 200, 100)


	DisplayScore(0);

}

//updates the score of the player 
void Score::DisplayScore(f32 points) 
{
	score += points;
	//scoreText->setText(stringw(score).c_str());
	
}
