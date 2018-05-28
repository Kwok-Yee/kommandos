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
IParticleSystemSceneNode* ps;
IParticleSystemSceneNode* muzzle;
IVideoDriver* particleDriver;

s32 particleTimer = 0;
s32 time;
s32 muzzleTimer = 0;

ParticleSystem::ParticleSystem(IrrlichtDevice* device)
{
	particleIDevice = device;
	particleSmgr = device->getSceneManager();
	ps = particleSmgr->addParticleSystemSceneNode(false);
	muzzle = particleSmgr->addParticleSystemSceneNode(false);
	particleDriver = device->getVideoDriver();
}


void ParticleSystem::Update()
{

	// Work out a frame delta time.
	const u32 now = particleIDevice->getTimer()->getTime();
	const f32 frameTime = (f32)(now - time) / 1000.f; // Time in seconds
	time = now;

	//for removing particle on position
	if (activePs && particleTimer > 0)
	{
		particleTimer -= frameTime;

		if (particleTimer <= 0)
		{
			activePs = false;
			ps->setEmitter(0);
			printf("EnemyTimeUp");
		}
	}
	//Inefficient way of stopping particle
	if (muzzleActive && muzzleTimer > 0)
	{
	//printf("Muzzle active");
		muzzleTimer -= frameTime;
		if (muzzleTimer <= 0)
		{
		printf("TimeUp");
			muzzle->setEmitter(0);
			muzzleActive = false;
			
		}
	}

}
//creates particles on the object position if "activePs" is true
void ParticleSystem::CreateParticles(vector3df Position, path texture)
{
	activePs = true;
	
	particleTimer = 1000;

	scene::IParticleEmitter * em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7, 0, -7, 7, 1, 7), // emitter size
		core::vector3df(0.0f, 0.06f, 0.0f),   // initial direction
		1, 10,                            // min and max particles per sec
		video::SColor(0, 255, 255, 255),       // darkest color
		video::SColor(0, 255, 255, 255),       // brightest color
		800, 2000, 0,                         // min and max age, angle
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

}


void ParticleSystem::CreateMuzzleFlash(vector3df position, path texture)
{
	muzzleActive = true;
	muzzleTimer = 1000;
	
	scene::IParticleEmitter * mf = muzzle->createBoxEmitter(
		core::aabbox3d<f32>(-3, 0, -3, 3, 1, 3), // emitter size
		core::vector3df(0.0f, 0.0f, 0.0f),   // initial direction
		1, 10,                             // emit rate
		video::SColor(0, 255, 255, 255),       // darkest color
		video::SColor(0, 255, 255, 255),       // brightest color
		800, 2000, 0,                         // min and max age, angle
		core::dimension2df(5.f, 5.f),         // min size
		core::dimension2df(5.f, 5.f));        // max size

	muzzle->setEmitter(mf); // this grabs the emitter
	mf->drop(); // so we can drop it here without deleting it

	scene::IParticleAffector* paf = muzzle->createFadeOutParticleAffector(irr::video::SColor(0, 0, 0, 0), 10000);

	muzzle->addAffector(paf); // same goes for the affector
	paf->drop();

	muzzle->setPosition(position);
	muzzle->setScale(core::vector3df(1, 1, 1));
	muzzle->setMaterialFlag(video::EMF_LIGHTING, false);
	muzzle->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	muzzle->setMaterialTexture(0, particleDriver->getTexture(texture));
	muzzle->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);


}
