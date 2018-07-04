#include "UISystem.h"
#include <irrlicht.h>
#include "EnemySpawner.h"
#include "Player.h"
#include "Game.h"

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

IGUIEnvironment* GUI_UISystem;
IVideoDriver* UIdriver;

IGUIFont* font2_UI;
IGUIImage* rapidImg;
IGUIImage* splitImg;
ITexture* rapidFireTexture;
ITexture* splitFireTexture;
SColor inActiveColor, activeColor, rapidUIColor, splitUIColor;

Game* game_UI;
Player* _Player;

EGUI_ALIGNMENT upperleft_UI, upperight_UI;

UISystem::UISystem(IrrlichtDevice* device)
{
	game_UI = game_UI->GetInstance();
}


void UISystem::InitUISystem(irr::IrrlichtDevice * device)
{
	game_UI = game_UI->GetInstance();
	_Player = game_UI->GetPlayer();

	UIdriver = game_UI->device->getVideoDriver();
	GUI_UISystem = game_UI->device->getGUIEnvironment();

	inActiveColor = SColor((u32)50, (u32)255, (u32)0, (u32)0);
	activeColor = SColor((u32)255, (u32)255, (u32)255, (u32)255);

	rapidFireTexture = UIdriver->getTexture("../media/Textures/rapid.png");
	rapidImg = GUI_UISystem->addImage(rapidFireTexture, position2d<int>(700, 500));
	rapidImg->setScaleImage(true);
	rapidImg->bringToFront(rapidImg);
	rapidImg->setMaxSize(dimension2du(50, 50));

	splitFireTexture = UIdriver->getTexture("../media/Textures/split.png");
	splitImg = GUI_UISystem->addImage(splitFireTexture, position2d<int>(700, 400));
	splitImg->setScaleImage(true);
	splitImg->bringToFront(rapidImg);
	splitImg->setMaxSize(dimension2du(50, 50));

	rapidUIColor = inActiveColor;
	splitUIColor = inActiveColor;
}

void UISystem::WaveUI(IrrlichtDevice* device)
{
	//Showing the current wave
	font2_UI = device->getGUIEnvironment()->getFont("../media/Fonts/fontlucida.png");
	core::stringw waveNumberString = L"Current wave: ";
	waveNumberString += currentWave;

	//For drawing the wave timer in seconds
	int waveCooldownSeconds = waveCooldown / 60;
	core::stringw waveCountDownString = L"Time till wave: ";
	waveCountDownString += waveCooldownSeconds;

	//Big wave incoming string
	core::stringw bigWaveString = L"BIG WAVE INCOMING!!";	

	if(game_UI->GetIsGameOver() == false)
	{
		if (_Player->GetRapidFireTimer() > 0)
		{
			rapidUIColor = activeColor;
		}
		else
		{
			rapidUIColor = inActiveColor;
		}
		if (_Player->GetSplitFireTimer() > 0)
		{
			splitUIColor = activeColor;
		}
		else
		{
			splitUIColor = inActiveColor;
		}

		rapidImg->setColor(rapidUIColor);
		splitImg->setColor(splitUIColor);
	}
	else 
	{
		GUI_UISystem->clear();
	}

	if (!game_UI->GetIsGameOver())
	{
		//Drawing the strings:
		font2_UI->draw(stringw(waveNumberString),
			core::rect<s32>(600, 40, 200, 100),
			video::SColor(255, 255, 255, 255));

		if (waveChangeUI == true)
		{
			font2_UI->draw(stringw(waveCountDownString),
				core::rect<s32>(300, 40, 200, 100),
				video::SColor(255, 255, 255, 255));

			if ((currentWave + 1) % 5 == 0)
			{
				font2_UI->draw(stringw(bigWaveString),
					core::rect<s32>(325, 120, 200, 100),
					video::SColor(255, 255, 255, 255));
			}
		}
	}

}
