#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"


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
	//box = App->textures->Load("pinball/crate.png");
	//bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	structure = App->textures->Load("pinball/Pinball_No_Margins.png");
	circle = App->textures->Load("pinball/Ball.png");
	texture_sensor = App->textures->Load("pinball/sensor_red.png");
	
	sensors.add(App->physics->CreateCircleSensor(1000, 800, 16));
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH*0.850, SCREEN_HEIGHT*1.7, 800, 400);
	ball_propeller = App->physics->CreateKinematicRectangle(1221, 990, 45, 130);

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
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 22));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		circles.clear();
	}
	// Draw pinball -------------------------------------------------------------
	int pinball[82] = {
		620, 665,
		620, 1286,
		668, 1285,
		664, 202,
		657, 161,
		637, 120,
		607, 89,
		565, 64,
		530, 55,
		190, 55,
		167, 56,
		134, 65,
		103, 83,
		80, 105,
		61, 134,
		53, 170,
		51, 206,
		55, 259,
		69, 304,
		88, 345,
		130, 400,
		24, 483,
		24, 825,
		31, 860,
		41, 882,
		59, 906,
		84, 927,
		135, 956,
		299, 1023,
		299, 1047,
		5, 1045,
		5, 1455,
		614, 1455,
		612, 1047,
		390, 1045,
		388, 1025,
		504, 980,
		570, 932,
		596, 895,
		601, 855,
		601, 667
	};
	pinballs.add(App->physics->CreateChain(SCREEN_WIDTH/2, 55, pinball, 83));

	//Walls
	int Wall1[32] = {
		3, 165,
		3, 7,
		7, 3,
		31, 3,
		34, 7,
		33, 126,
		40, 145,
		55, 163,
		75, 180,
		126, 214,
		118, 217,
		112, 228,
		111, 237,
		113, 244,
		110, 245,
		6, 169
	};
	pops_triangle.add(App->physics->CreateChain(570, 722, Wall1, 33));
	int Wall2[32] = {
		41, 11,
		44, 7,
		66, 7,
		70, 11,
		70, 172,
		26, 215,
		24, 214,
		25, 204,
		20, 192,
		12, 186,
		5, 186,
		20, 173,
		31, 160,
		39, 147,
		41, 133,
		41, 21
	};
	pops_triangle.add(App->physics->CreateChain(1007, 751, Wall2, 33));

	//Bumpers
	//Cricles
	pops_circular.add(App->physics->CreateStaticCircle(724, 350, 45));
	pops_circular.add(App->physics->CreateStaticCircle(985, 371, 45));
	pops_circular.add(App->physics->CreateStaticCircle(854, 564, 45));

	//Triangles
	int Tri1[64] = {
		269, 66,
		271, 58,
		268, 50,
		85, 12,
		77, 12,
		13, 49,
		10, 59,
		16, 67,
		31, 68,
		27, 91,
		44, 91,
		40, 68,
		71, 68,
		68, 90,
		83, 90,
		80, 68,
		115, 68,
		112, 90,
		128, 90,
		125, 68,
		158, 68,
		156, 90,
		172, 90,
		168, 68,
		200, 68,
		197, 91,
		214, 91,
		211, 68,
		243, 68,
		240, 91,
		256, 91,
		253, 68
	};
	pops_triangle.add(App->physics->CreateChain(702, 159, Tri1, 65));
	int Tri2[16] = {
		78, 84,
		34, 21,
		24, 17,
		17, 24,
		17, 152,
		25, 160,
		36, 154,
		79, 91
	};
	pops_triangle.add(App->physics->CreateChain(985, 480, Tri2, 17));
	int Tri3[16] = {
		9, 96,
		36, 21,
		44, 14,
		53, 19,
		86, 144,
		81, 151,
		72, 152,
		12, 101
	};
	pops_triangle.add(App->physics->CreateChain(627, 710, Tri3, 17));

	//Square
	int Sqr[32] = {
		22, 91,
		17, 95,
		11, 87,
		105, 8,
		112, 16,
		105, 22,
		120, 36,
		108, 46,
		97, 28,
		68, 52,
		82, 66,
		69, 77,
		59, 60,
		30, 84,
		43, 99,
		32, 108
	};
	pops_square.add(App->physics->CreateChain(536, 445, Sqr, 33));

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = pinballs.getFirst();

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

	c = circles.getFirst();

	while (c != NULL)
	{
		int x, y; 
		c->data->GetPosition(x, y);
		//if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
		App->renderer->Blit(circle, x, y, NULL, 2.0f, c->data->GetRotation());
		c = c->next;
	}

	c = sensors.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
		{
			App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
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

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
