#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace io;

class ParticleSystem
{
public:
	void SystemParticle(irr::IrrlichtDevice* device);
	void CreateParticles(vector3df Position);

	bool hit = false;
};

