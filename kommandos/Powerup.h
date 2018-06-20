#pragma once

class Powerup
{
public:

	void Reset();
	irr::scene::ISceneNode* GetPowerup();
	void SetPowerup(irr::scene::ISceneNode* newPowerup);
	irr::s32 GetPowerupType();
	Powerup();

private:

	irr::scene::ISceneNode* powerup;
	irr::s32 type; //0 = default, 1 = splitshot, 2 = firerate

};

