#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "SDL\include\SDL_timer.h"

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
	
	//Bumpers
	p2List<PhysBody*> pops_circular;
	p2List<PhysBody*> pops_triangle;
	p2List<PhysBody*> pops_square;
	p2List<PhysBody*> pops_mini;

	p2List<PhysBody*> flipper;

	//Sensors
	PhysBody* sensors1;
	PhysBody* sensors2;
	PhysBody* sensors3;
	PhysBody* sensor;
	bool sensed;
	bool tp = false;
	bool active1 = false;
	bool active2 = false;
	bool active3 = false;
	int active_sensors = 0;
	int score = 0;
	int time_now;
	int start_time;
	int total_time = NULL;
	bool reset = false;
	

	PhysBody* spring;
	PhysBody* spring2;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* structure;
	SDL_Texture* texture_sensor;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

public:
	uint bouncers_fx;
	uint spring_fx;
	uint flipper_fx;
};
