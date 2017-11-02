#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"

class PhysBody;
struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	iPoint position;

private:
	void setSpring();
	void setLeftFlipper();
	void setRightFlipper();

public:
	SDL_Texture* tx_spring;

	SDL_Texture* tx_flipper_left;
	SDL_Texture* tx_flipper_right;

private:
	PhysBody* spring;
	PhysBody* spring_pivot;

	PhysBody* flipper_left;
	PhysBody* flipper_pivot_left;
	PhysBody* flipper_right;
	PhysBody* flipper_pivot_right;

	b2PrismaticJoint* prismatic_joint;
	b2RevoluteJoint* revolute_joint_left;
	b2RevoluteJoint* revolute_joint_right;

public:
	void ModulePlayer::setSensor(PhysBody* sensor, SDL_Texture* texture, bool active);
};