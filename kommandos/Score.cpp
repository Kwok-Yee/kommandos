#include "Score.h"
#include <irrlicht.h>
#include "Game.h"


using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IGUIEnvironment* scoreGui;
IGUIFont* font2;
IGUIStaticText* scoreText;

Game* game_Score;

EGUI_ALIGNMENT upperleft,upperright;
s32 score = 0;
u32 timer;

//method for displaying score
void Score::Scoring(IrrlichtDevice* device) 
{
	game_Score = game_Score->GetInstance();
	scoreGui = device->getGUIEnvironment();
	timer = device->getTimer()->getTime();
	font2 = device->getGUIEnvironment()->getFont("../media/Fonts/fontlucida.png");
	core::stringw scoreString = L"Score: ";
	scoreString += score;

	if (game_Score->GetIsGameOver() != true)
	{
	font2->draw(stringw(scoreString).c_str(),
		core::rect<s32>(80, 40, 200, 100),
		video::SColor(255, 255, 255, 255));
	DisplayScore(0);
	}
}

//updates the score of the player 
void Score::DisplayScore(f32 points) 
{
	score += points;	
}
