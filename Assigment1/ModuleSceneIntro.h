#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> pinballs;
	p2List<PhysBody*> walls;
	PhysBody* sensors;
	PhysBody* sensors2;
	PhysBody* sensors3;
	bool active = false;
	bool tp = false;
	
	//Bumpers
	p2List<PhysBody*> pops_circular;
	p2List<PhysBody*> pops_triangle;
	p2List<PhysBody*> pops_square;

	p2List<PhysBody*> flipper;

	PhysBody* spring;
	PhysBody* spring2;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* structure;
	SDL_Texture* texture_sensor;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
