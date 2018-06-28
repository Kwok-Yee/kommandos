#pragma once
#include <irrlicht.h>
#include"ObjectPlacementGeneration.h"

class Game
{
public:
	irr::IrrlichtDevice * device;
	ObjectPlacementGeneration objectPlacementGen;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	//Returns the Game instance
	static Game* GetInstance();
	bool GetIsGameOver();
	bool SetIsGameOver(bool state);
	//initialize all objects.
	void Start();
	//Updates all objects in the game.
	void Update();
	//Draws everything in the game.
	void Draw();
private:
	Game();
	static Game* instance;
	bool isGameOver = false;
};

