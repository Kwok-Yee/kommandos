#pragma once

class Powerup
{
public:

	enum PowerupType
	{
		health, firerate, splitshot, base
	};

	void Reset();
	irr::scene::ISceneNode* GetNode();
	void SetPowerup(irr::scene::ISceneNode* newPowerup);
	irr::s32 GetPowerupType();
	Powerup(irr::IrrlichtDevice* device);
	void SetPowerupType(irr::s32 powType);

private:

	irr::scene::ISceneNode* powerup;
	PowerupType type;

};

