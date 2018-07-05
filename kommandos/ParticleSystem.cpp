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
	f32 particleTimer = 0.6f;

	scene::IParticleEmitter * em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-3, 0, -3, 3, 1, 3), // emitter size
		core::vector3df(0.0f, 0.03f, 0.0f),   // initial direction
		200, 300,                            // min and max particles per sec
		video::SColor(0, 255, 255, 255),       // darkest color
		video::SColor(0, 255, 255, 255),       // brightest color
		450, 500, 90,                         // min and max age, angle
		core::dimension2df(1.0f, 1.0f),         // min size texture
		core::dimension2df(1.3f, 1.3f));        // max size texture

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
