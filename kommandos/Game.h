#pragma once
#include <irrlicht.h>

class Game
{
public:
	irr::IrrlichtDevice * device;
	Game();
	void Start();
	void Update();
	void Draw();
	~Game();
};

