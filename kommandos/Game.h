#pragma once
#include <irrlicht.h>

class Game
{
public:
	irr::IrrlichtDevice * device;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	static Game* GetInstance();
	void Start();
	void Update();
	void Draw();
private:
	Game();
	static Game* instance;
};

