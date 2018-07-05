#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace io;

class ParticleSystem
{
public:
	ParticleSystem(irr::IrrlichtDevice* device);
	void Update(f32 frameDeltaTime);
	void CreateParticles(vector3df Position, path);

};
struct Particle
{
	scene::IParticleSystemSceneNode* system;
	f32 lifetime;
	Particle(scene::IParticleSystemSceneNode* _System, f32 _Lifetime) {
		this->system = _System;
		this->lifetime = _Lifetime;
	}
	void Update(f32 frameDeltaTime) {
		this->lifetime -= frameDeltaTime;
		if (lifetime <= 0) {
			system->remove();
		}
	}
};

