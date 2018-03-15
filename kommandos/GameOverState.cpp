#include "GameOverState.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

GameOverState::GameOverState()
{
}

void GameOverState::ShowGameOver(IrrlichtDevice* device)
{
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();
	IMesh* gameover = smgr->getMesh("../media/GameOverScreen.3ds");
	IMeshSceneNode* gameOverNode = smgr->addMeshSceneNode(gameover);
	if (gameOverNode) {

		gameOverNode->setScale(vector3df(50, 50, 50));
		gameOverNode->setMaterialTexture(0, driver->getTexture("../media/Game_Over_Screen.jpg"));
		gameOverNode->setRotation(vector3df(0, -90, 0));
		gameOverNode->setMaterialFlag(video::EMF_LIGHTING, false);
	}
}

