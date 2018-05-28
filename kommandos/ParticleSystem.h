#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace io;

class ParticleSystem
{
public:
	ParticleSystem(irr::IrrlichtDevice* device);
	void Update();
	void CreateParticles(vector3df Position, path);
	void CreateMuzzleFlash(vector3df, path);

	bool activePs = false;// activePs = Active ParticleSystem
	bool muzzleActive = false;
};

