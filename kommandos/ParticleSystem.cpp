#include "ParticleSystem.h"
#include <irrlicht.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

IrrlichtDevice* partcleIDevice;
ISceneManager* partcleSmgr;
IParticleSystemSceneNode* ps;
IVideoDriver* partcleDriver;

const path text = "";
s32 setParticleTimer = 1;
s32 time;

ParticleSystem::ParticleSystem()
{
}

void ParticleSystem::SystemParticle(IrrlichtDevice* device) 
{
	partcleIDevice = device;
	partcleSmgr = device->getSceneManager();
	ps = partcleSmgr->addParticleSystemSceneNode(false);
	partcleDriver = device->getVideoDriver();

	// Work out a frame delta time.
	const u32 now = partcleIDevice->getTimer()->getTime();
	const f32 frameTime = (f32)(now - time) / 1000.f; // Time in seconds
	time = now;

	//if enemy is hit then create bloodparticle
	if (hit) 
	{
		CreateParticles(vector3df(20,20,20),text);
		setParticleTimer = 100;
	}

	if (hit && setParticleTimer > 0) 
	{ 
		setParticleTimer = setParticleTimer -= frameTime; 
		hit = false;

	}
}
//creates particles on the object position if "hit" is true
void ParticleSystem::CreateParticles(vector3df Position, path texture)
{
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
		ps->setMaterialTexture(0, partcleDriver->getTexture(texture));
		ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

		//if timer is "0" then remove particle
		//if (setParticleTimer <= 0)
		//{
		//	ps->setEmitter(0);
		//}

}
