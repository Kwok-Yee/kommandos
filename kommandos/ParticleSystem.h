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

	bool hit = false;
};

