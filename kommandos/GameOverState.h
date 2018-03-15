#pragma once
#include <irrlicht.h>

class GameOverState
{
public:
	 GameOverState();
	 //Shows gameover Screens
	void ShowGameOver(irr::scene::ISceneManager* smgr, irr::video::IVideoDriver* driver);

};

