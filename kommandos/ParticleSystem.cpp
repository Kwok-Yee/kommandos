#include <irrlicht.h>
#include "ParticleSystem.h"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace std;

IrrlichtDevice* particleIDevice;
ISceneManager* particleSmgr;
IVideoDriver* particleDriver;
core::array<Particle*> particles;


ParticleSystem::ParticleSystem(IrrlichtDevice* device)
{
	particleIDevice = device;
	particleSmgr = device->getSceneManager();
	particleDriver = device->getVideoDriver();
}

void ParticleSystem::Update(f32 frameDeltaTime)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Update(frameDeltaTime);
		if (particles[i]->lifetime <= 0) {
			particles.erase(i);
		}
	}
}

//creates particles on the object position
void ParticleSystem::CreateParticles(vector3df Position, path texture)
{

	IParticleSystemSceneNode* ps = particleSmgr->addParticleSystemSceneNode(false);
	f32 particleTimer = 5;

	scene::IParticleEmitter * em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7, 0, -7, 7, 1, 7), // emitter size
		core::vector3df(0.0f, 0.0f, 0.0f),   // initial direction
		10, 20,                            // min and max particles per sec
		video::SColor(0, 255, 255, 255),       // darkest color
		video::SColor(0, 255, 255, 255),       // brightest color
		800, 1500, 0,                         // min and max age, angle
		core::dimension2df(0.5f, 0.5f),         // min size texture
		core::dimension2df(1.f, 1.f));        // max size texture

	ps->setEmitter(em); // this grabs the emitter, if "0" then it stop creating particles
	em->drop(); // so we can drop it here without deleting it

	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();

	ps->addAffector(paf); // same goes for the affector
	paf->drop();

	ps->setPosition(Position);
	ps->setScale(core::vector3df(1, 1, 1));
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, particleDriver->getTexture(texture));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	particles.push_front(new Particle(ps, particleTimer));
}
