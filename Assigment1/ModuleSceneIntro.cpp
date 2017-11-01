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



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = structure = NULL;
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
	//box = App->textures->Load("pinball/crate.png");
	//bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	
	//Sensors
	sensors1 = App->physics->CreateCircleSensor(855, 825, 16, 0); 
	sensors1->listener = this;
	sensors2 = App->physics->CreateCircleSensor(855, 868, 16, 0);
	sensors2->listener = this;
	sensors3 = App->physics->CreateCircleSensor(855, 909, 16, 0);
	sensors3->listener = this;
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH*0.850, SCREEN_HEIGHT*1.7, 800, 400);
	sensor->listener = this;

	//Draw Ball
	circles.add(App->physics->CreateCircle(1150, 800, 16, 0.15f, b2_dynamicBody));
	circles.getLast()->data->listener = this;

	// Draw pinball ------------------------------------------------------------------------
	int pinball[82] = {
		620, 172, 620, 1286, 666, 1286,666, 203,655, 161,636, 122,606, 90,565, 66,529, 57,183, 57,
		159, 60,132, 67,103, 83,80, 105,61, 134,53, 170,51, 206,56, 260,69, 304,88, 345,129, 401,24, 487,
		24, 831,31, 860,41, 882,65, 912,100, 938,155, 966,300, 1021,300, 1047,5, 1047,5, 1455,
		612, 1455,	612, 1047,	384, 1047,384, 1021,504, 975,566, 931,588, 904,598, 873,598, 172
	};
	pinballs.add(App->physics->CreateChain(SCREEN_WIDTH / 2, 55, pinball, 83, 0.0f, b2_staticBody));

	//Walls
	int Wall1[32] = {
		14, 267, 14, 30, 16, 24, 41, 24, 44, 30, 44, 234, 50, 252, 63, 267,
		79, 283, 136, 321, 129, 324, 124, 333, 121, 344, 125, 351, 122, 355, 14, 276
	};
	pops_triangle.add(App->physics->CreateChain(559, 615, Wall1, 33, 0.0f, b2_staticBody));

	int Wall2[32] = {
		41, 16, 45, 13, 66, 13, 70, 17, 70, 237, 27, 280, 24, 281, 25, 270,
		21, 259, 15, 254, 6, 252, 27, 231, 35, 221, 41, 207, 41, 133, 41, 21
	};
	pops_triangle.add(App->physics->CreateChain(1006, 685, Wall2, 33, 0.0f, b2_staticBody));

	//Bumpers ------------------------------------------------------------------------------
	//Cricles
	pops_circular.add(App->physics->CreateCircle(724, 350, 45, 0.5f, b2_staticBody));
	pops_circular.add(App->physics->CreateCircle(985, 371, 45, 0.5f, b2_staticBody));
	pops_circular.add(App->physics->CreateCircle(854, 564, 45, 0.5f, b2_staticBody));

	//Triangles
	int Tri1[64] = {
		269, 66, 271, 58, 268, 50, 85, 12, 77, 12, 13, 49, 10, 59, 16, 67, 31, 68,
		27, 91, 44, 91, 52, 68, 60, 68, 68, 90, 83, 90, 94, 68, 101, 68, 112, 90, 128, 90,
		137, 68, 147, 68, 156, 90, 172, 90, 180, 68, 188, 68, 197, 91, 214, 91,
		220, 68, 233, 68, 240, 91, 256, 91, 253, 68
	};
	pops_triangle.add(App->physics->CreateChain(702, 159, Tri1, 65, 0.8f, b2_staticBody));

	int Tri2[16] = {
		78, 84, 34, 21, 24, 17, 17, 24,
		17, 152, 25, 160, 36, 154, 79, 91
	};
	pops_triangle.add(App->physics->CreateChain(985, 480, Tri2, 17, 0.8f, b2_staticBody));

	int Tri3[16] = {
		9, 96, 36, 21, 44, 14, 53, 19,
		86, 144, 81, 151, 72, 152, 12, 101
	};
	pops_triangle.add(App->physics->CreateChain(628, 710, Tri3, 17, 0.8f, b2_staticBody));

	//Squares
	int Sqr[32] = {
		22, 91, 17, 95, 11, 87, 105, 8, 112, 16, 105, 22, 120, 36, 108, 46,
		87, 36, 77, 44, 82, 66, 69, 77, 48, 68, 39, 76, 43, 99, 32, 108
	};
	pops_square.add(App->physics->CreateChain(536, 445, Sqr, 33, 0.8f, b2_staticBody));
	
	pops_square.add(App->physics->CreateRectangle(554, 725, 23, 25, 3.0f, b2_staticBody));
	pops_square.add(App->physics->CreateRectangle(1093, 758, 23, 25, 3.0f, b2_staticBody));

	//Load audio ------------------------------------------------------------------------
	App->audio->PlayMusic("Audio/Music_audio.ogg", 1.0f);

	bouncers_fx = App->audio->LoadFx("audio/Bumpers_fx.wav");
	flipper_fx = App->audio->LoadFx("audio/Flipper_fx.wav");
	spring_fx = App->audio->LoadFx("audio/Spring_fx.wav");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	char sscore[64] = "hihi";
	char lives[4] = "1";
	char Title[64] = "Score: ";
	sprintf_s(Title, "%d ", score);
	//strcat_s(Title,score);

	App->window->SetTitle(Title);

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = pinballs.getFirst();
	PhysBody* b;

	SDL_Rect rect;

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(structure, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	b = sensors1;

	if (b != NULL)
	{
		int x, y;
		b->GetPosition(x, y);
		if (reset == true)
		{
			b->body->SetAwake(true);
			active1 = false;
		}
		if (b->body->IsAwake() == false)
		{
			App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
		}
		if (active1 == true && b->body->IsAwake() == true && active_sensors < 3)
		{
			b->body->SetAwake(false);
			active_sensors++;
			active1 = false;
			//App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
		}
	}

	b = sensors2;

	if (b != NULL)
	{
		int x, y;

		b->GetPosition(x, y);
		if (reset == true)
		{
			b->body->SetAwake(true);
			active2 = false;
		}
		if (b->body->IsAwake() == false)
		{
			App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
		}
		if (active2 == true && b->body->IsAwake() == true && active_sensors < 3)
		{
			b->body->SetAwake(false);
			active_sensors++;
			active2 = false;
			//App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
		}
	}

	b = sensors3;

	if (b != NULL)
	{
		int x, y;

		b->GetPosition(x, y);
		if (reset == true)
		{
			b->body->SetAwake(true);
			active3 = false;
			reset = false;
		}
		if (b->body->IsAwake() == false)
		{
			App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
		}
		if (active3 == true && b->body->IsAwake() == true && active_sensors < 3)
		{
			b->body->SetAwake(false);
			active_sensors++;
			active3 = false;
			//App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		//circles.clear();
	
	}
	time_now = SDL_GetTicks() - start_time;
	if (active_sensors < 3)
	{
	
		total_time = time_now + (Uint32)(3000.0f);;
	}
	else
	{
		if (time_now > total_time)
		{
			start_time = SDL_GetTicks();
			score = score + 10;
			active_sensors = 0;
			reset = true;
		}
	}

	c = circles.getFirst();

	while (c != NULL)
	{
		int x, y; 
		c->data->GetPosition(x, y);
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			b2Vec2 position(23.1f, 16.9f);
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
			b2Vec2 position(23.1f, 16.9f);
			c->data->body->SetTransform(position, 0);
			tp = false;
		}
		c = c->next;
	}



	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;
		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);
		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	if(bodyA == sensors1)
	{
		active1 = true;
		//b2Vec2 positionm(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
		//bodyA->GetPosition(x, y);
		//App->renderer->DrawCircle(x, y, 50, 200, 100, 100);
	}
	if (bodyA == sensors2)
	{
		active2 = true;
	}
	if (bodyA == sensors3)
	{
		active3 = true;
	}
	if (bodyA == sensor)
	{
		tp = true;
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
}
