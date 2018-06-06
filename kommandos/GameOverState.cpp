#include "GameOverState.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;

vector3df objScale (100,100,100);
vector3df objPosition(-25, 0, 20);
vector3df objRotate(0, -90, 0);
const path gameOver = "../media/Models/arena/GameOverScreen.3ds";

//creates a scenenode for showing a gameoverscreen
void GameOverState::ShowGameOver(IrrlichtDevice* device)
{
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();
	IMesh* gameover = smgr->getMesh(gameOver);
	IMeshSceneNode* gameOverNode = smgr->addMeshSceneNode(gameover);
	if (gameOverNode) {

		gameOverNode->setScale(objScale);
		gameOverNode->setPosition(objPosition);
		gameOverNode->setMaterialTexture(0, driver->getTexture("../media/Textures/Game_Over_Screen.jpg"));
		gameOverNode->setRotation(objRotate);
		gameOverNode->setMaterialFlag(video::EMF_LIGHTING, false);
	}
}

