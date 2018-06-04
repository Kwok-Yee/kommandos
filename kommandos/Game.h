#pragma once
#include <irrlicht.h>
#include "Player.h"
#include "ObjectPlacementGeneration.h"

class Game
{
public:
	irr::IrrlichtDevice* device;
	Player* player;
	ObjectPlacementGeneration objectPlacementGen;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	//Returns the Game instance
	static Game* GetInstance();
	//initialize all objects.
	void Start();
	//Updates all objects in the game.
	void Update();
	//Draws everything in the game.
	void Draw();
	bool GetIsGameOver();
	bool SetIsGameOver(bool state);
private:
	Game();
	static Game* instance;
};

