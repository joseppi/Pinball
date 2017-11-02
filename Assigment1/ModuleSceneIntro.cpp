#include <string.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleWindow.h"
#include "p2SString.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = structure = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	//Load Textures
	structure = App->textures->Load("pinball/Pinball_No_Margins.png");
	circle = App->textures->Load("pinball/Ball.png");
	texture_sensor = App->textures->Load("pinball/sensor_red.png");
	texture_sensor_circs = App->textures->Load("pinball/sensor_circs.png");


	//bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	
	//RED Sensors
	sensors1 = App->physics->CreateCircleSensor(343, 770, 16, 0); 
	sensors1->listener = this;
	sensors2 = App->physics->CreateCircleSensor(343, 812, 16, 0);
	sensors2->listener = this;
	sensors3 = App->physics->CreateCircleSensor(343, 853, 16, 0);
	sensors3->listener = this;

	//BOUNCERS (CIRCLES) SENSORS
	sensor_circ1 = App->physics->CreateCircleSensor(212, 295, 50, 0);
	sensor_circ1->listener = this;
	sensor_circ2 = App->physics->CreateCircleSensor(473, 316, 50, 0);
	sensor_circ2->listener = this;
	sensor_circ3 = App->physics->CreateCircleSensor(342, 509, 50, 0);
	sensor_circ3->listener = this;

	tp_sensor = App->physics->CreateRectangleSensor(310, 1275, 590, 400);
	tp_sensor->listener = this;

	//Draw Ball
	circles.add(App->physics->CreateCircle(643, 745, 16, 0.15f, b2_dynamicBody));
	circles.getLast()->data->listener = this;

	// Draw pinball ------------------------------------------------------------------------
	int pinball[82] = {
		620, 172, 620, 1286, 666, 1286,666, 203,655, 161,636, 122,606, 90,565, 66,529, 57,183, 57,
		159, 60,132, 67,103, 83,80, 105,61, 134,53, 170,51, 206,56, 260,69, 304,88, 345,129, 401,24, 487,
		24, 831,31, 860,41, 882,65, 912,100, 938,155, 966,300, 1021,300, 1047,5, 1047,5, 1455,
		612, 1455,	612, 1047,	384, 1047,384, 1021,504, 975,566, 931,588, 904,598, 873,598, 172
	};
	pinballs.add(App->physics->CreateChain(0, 0, pinball, 83, 0.0f, b2_staticBody));

	//Walls
	int Wall1[32] = {
		14, 182, 14, 25, 17, 19, 40, 19, 43, 26, 43, 134, 45, 150, 52, 162, 73, 186,
		136, 228, 130, 232, 123, 242, 121, 250, 124, 259, 122, 262, 26, 192
	};
	pops_triangle.add(App->physics->CreateChain(48, 653, Wall1, 33, 0.0f, b2_staticBody));

	int Wall2[32] = {
		41, 13, 46, 9, 66, 9, 70, 13, 70, 173, 26, 217, 24, 216,
		25, 209, 22, 199, 15, 190, 5, 188, 28, 165, 36, 154, 41, 141, 41, 54, 41, 23
	};
	pops_triangle.add(App->physics->CreateChain(494, 694, Wall2, 33, 0.0f, b2_staticBody));

	//Bumpers ------------------------------------------------------------------------------
	//Cricles
	pops_circular.add(App->physics->CreateCircle(212, 295, 45, 0.5f, b2_staticBody));
	pops_circular.add(App->physics->CreateCircle(473, 316, 45, 0.5f, b2_staticBody));
	pops_circular.add(App->physics->CreateCircle(342, 509, 45, 0.5f, b2_staticBody));

	//Triangles
	int Tri1[64] = {
		269, 66, 271, 58, 268, 50, 85, 12, 77, 12, 13, 49, 10, 59, 16, 67, 31, 68,
		27, 91, 44, 91, 52, 68, 60, 68, 68, 90, 83, 90, 94, 68, 101, 68, 112, 90, 128, 90,
		137, 68, 147, 68, 156, 90, 172, 90, 180, 68, 188, 68, 197, 91, 214, 91,
		220, 68, 233, 68, 240, 91, 256, 91, 253, 68
	};
	pops_triangle.add(App->physics->CreateChain(190, 104, Tri1, 65, 0.8f, b2_staticBody));

	int Tri2[16] = {
		78, 84, 34, 21, 24, 17, 17, 24,
		17, 152, 25, 160, 36, 154, 79, 91
	};
	pops_triangle.add(App->physics->CreateChain(470, 425, Tri2, 17, 0.8f, b2_staticBody));

	int Tri3[16] = {
		9, 96, 36, 21, 44, 14, 53, 19,
		86, 144, 81, 151, 72, 152, 12, 101
	};
	pops_triangle.add(App->physics->CreateChain(116, 655, Tri3, 17, 0.8f, b2_staticBody));

	//Square
	int Sqr[32] = {
		22, 91, 17, 95, 11, 87, 105, 8, 112, 16, 105, 22, 120, 36, 108, 46,
		87, 36, 77, 44, 82, 66, 69, 77, 48, 68, 39, 76, 43, 99, 32, 108
	};
	pops_square.add(App->physics->CreateChain(24, 391, Sqr, 33, 0.8f, b2_staticBody));
	
	//Mini
	pops_mini.add(App->physics->CreateCircle(42, 1670, 15, 3.0f, b2_staticBody));
	pops_mini.add(App->physics->CreateCircle(581, 1703, 15, 3.0f, b2_staticBody));

	//Load audio ------------------------------------------------------------------------
	//App->audio->PlayMusic("Audio/Music_audio.ogg", 1.0f);

	bouncers_fx = App->audio->LoadFx("audio/Bumpers_fx.wav");
	flipper_fx = App->audio->LoadFx("audio/Flipper_fx.wav");
	spring_fx = App->audio->LoadFx("audio/Spring_fx.wav");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(structure);
	App->textures->Unload(circle);
	App->textures->Unload(texture_sensor);
	App->textures->Unload(texture_sensor_circs);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	p2SString title("Pinball Score: %d  Lives: %d", score, lives);
	App->window->SetTitle(title.GetString());

	// All draw functions ------------------------------------------------------
	c = pinballs.getFirst();

	SDL_Rect rect;

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(structure, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		lives = 3;
		score = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)//Game over Screen
	{
		lives = 0;
	}
	//RED SENSORS
	App->player->setSensorCirclesRed(sensors1, texture_sensor, active1, active_sensors, reset_red_sensors);
	App->player->setSensorCirclesRed(sensors2, texture_sensor, active2, active_sensors, reset_red_sensors);
	App->player->setSensorCirclesRed(sensors3, texture_sensor, active3, active_sensors, reset_red_sensors);

	//--Delay
	time_now1 = SDL_GetTicks() - start_time1;
	if (active_sensors < 3)
	{
		total_time1 = time_now1 + (Uint32)(1000.0f);;
	}
	else
	{
		if (time_now1 > total_time1)
		{
			start_time1 = SDL_GetTicks();
			score += 10;
			active_sensors = 0;
			reset_red_sensors = true;
		}
	}

	//BOUNCERS (CIRCLES) SENSORS
	App->player->setSensorCircles(sensor_circ1, texture_sensor_circs, active_circ1);
	App->player->setSensorCircles(sensor_circ2, texture_sensor_circs, active_circ2);
	App->player->setSensorCircles(sensor_circ3, texture_sensor_circs, active_circ3);

	//--Delay 1
	time_now_circ1 = SDL_GetTicks() - start_time_circ1;
	if (active_circ1 == false){
		total_time_circ1 = time_now_circ1 + (Uint32)(250.0f);;
	}
	else
	{
		if (time_now_circ1 > total_time_circ1)
		{ start_time_circ1 = SDL_GetTicks(); score += 100; active_circ1 = false; b->body->SetAwake(true);}
	}

	//--Delay 2
	time_now_circ2 = SDL_GetTicks() - start_time_circ2;
	if (active_circ2 == false) {
		total_time_circ2 = time_now_circ2 + (Uint32)(250.0f);;
	}
	else
	{
		if (time_now_circ2 > total_time_circ2) 
		{ start_time_circ2 = SDL_GetTicks(); score += 100; active_circ2 = false; b->body->SetAwake(true); }
	}

	//--Delay 3
	time_now_circ3 = SDL_GetTicks() - start_time_circ3;
	if (active_circ3 == false) {
		total_time_circ3 = time_now_circ3 + (Uint32)(250.0f);;
	}
	else
	{
		if (time_now_circ3 > total_time_circ3) 
		{ start_time_circ3 = SDL_GetTicks(); score += 100; active_circ3 = false; b->body->SetAwake(true); }
	}

	c = circles.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{

			b2Vec2 position(PIXEL_TO_METERS(643), PIXEL_TO_METERS(795));
			c->data->body->SetLinearVelocity({ 0,0 });
			c->data->body->SetAngularVelocity(0);
			c->data->body->SetTransform(position, 0);
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			b2Vec2 positionm(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
			c->data->body->SetTransform(positionm, 0);
		}
		App->renderer->Blit(circle, x - 6, y - 6, NULL, 2.0f, c->data->GetRotation());
		if (tp == true)
		{
			b2Vec2 position(PIXEL_TO_METERS(643), PIXEL_TO_METERS(795));
			c->data->body->SetLinearVelocity({ 0,0 });
			c->data->body->SetAngularVelocity(0);
			c->data->body->SetTransform(position, 0);
			if (lives > 0)
			{
				--lives;
			}
			tp = false;
		}
		c = c->next;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
		App->audio->RiseVolume();

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
		App->audio->LowerVolume();

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		App->audio->MuteVolume();

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

    //RED SENSORS
	{
		if (bodyA == sensors1)
		{
			active1 = true;
		}
		if (bodyA == sensors2)
		{
			active2 = true;
		}
		if (bodyA == sensors3)
		{
			active3 = true;
		}
	}

	//BOUNCERS (CIRCLES) SENSORS
	{
		if (bodyA == sensor_circ1)
		{
			active_circ1 = true;
		}
		if (bodyA == sensor_circ2)
		{
			active_circ2 = true;
		}
		if (bodyA == sensor_circ3)
		{
			active_circ3 = true;
		}
	}
	

	if (bodyA == tp_sensor)
	{
		tp = true;
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
}
