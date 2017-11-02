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
	p2List_item<PhysBody*>* c;
	PhysBody* b;

	//----------------------------------------------------

	p2List<PhysBody*> circles;
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

	PhysBody* tp_sensor;
	PhysBody* spring_sensor;

	PhysBody* sensor_circ1;
	PhysBody* sensor_circ2;
	PhysBody* sensor_circ3;
	
	bool active1 = false;
	bool active2 = false;
	bool active3 = false;
	int active_sensors = 0;

	bool active1_circ1 = false;
	bool active1_circ2 = false;
	bool active1_circ3 = false;

	bool sensed;
	bool tp = false;

	int score = 0;
	int lives = 3;
	int time_now1, time_now2;
	int start_time1, start_time2;
	int total_time1, total_time2 = NULL;
	bool reset_red_sensors = false;
	bool reset_pops = false;
	
	SDL_Texture* circle;
	SDL_Texture* structure;
	SDL_Texture* texture_sensor;

	SDL_Texture* texture_sensor_circs;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

public:
	uint bouncers_fx;
	uint spring_fx;
	uint flipper_fx;
};
