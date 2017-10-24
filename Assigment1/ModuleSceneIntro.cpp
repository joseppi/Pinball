#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"


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
	
	sensors.add(App->physics->CreateCircleSensor(1000, 800, 16, 0));
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH*0.850, SCREEN_HEIGHT*1.7, 800, 400);

	//Sensors
	sensors.add(App->physics->CreateCircleSensor(855, 825, 16, 0));
	sensors.add(App->physics->CreateCircleSensor(855, 868, 16, 0));
	sensors.add(App->physics->CreateCircleSensor(855, 909, 16, 0));

	//Draw Ball
	circles.add(App->physics->CreateCircle(1150, 800, 16, b2_dynamicBody));
	circles.getLast()->data->listener = this;

	// Draw pinball -------------------------------------------------------------
	int pinball[82] = {
		620, 665,
		620, 1286,
		666, 1286,
		666, 203,
		655, 161,
		636, 122,
		606, 90,
		565, 66,
		529, 57,
		183, 57,
		159, 60,
		132, 67,
		103, 83,
		80, 105,
		61, 134,
		53, 170,
		51, 206,
		56, 260,
		69, 304,
		88, 345,
		129, 401,
		24, 487,
		24, 831,
		31, 860,
		41, 882,
		65, 912,
		100, 938,
		155, 966,
		300, 1021,
		300, 1047,
		5, 1047,
		5, 1455,
		612, 1455,
		612, 1047,
		384, 1047,
		384, 1021,
		504, 975,
		566, 931,
		588, 904,
		598, 873,
		598, 665
	};
	pinballs.add(App->physics->CreateChain(SCREEN_WIDTH / 2, 55, pinball, 83, b2_staticBody));

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
	pops_triangle.add(App->physics->CreateChain(569, 722, Wall1, 33, b2_staticBody));

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
	pops_triangle.add(App->physics->CreateChain(1006, 751, Wall2, 33, b2_staticBody));

	//Bumpers -------------------------
	//Cricles
	pops_circular.add(App->physics->CreateCircle(724, 350, 45, b2_staticBody));
	pops_circular.add(App->physics->CreateCircle(985, 371, 45, b2_staticBody));
	pops_circular.add(App->physics->CreateCircle(854, 564, 45, b2_staticBody));

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
		52, 68,
		60, 68,
		68, 90,
		83, 90,
		94, 68,
		101, 68,
		112, 90,
		128, 90,
		137, 68,
		147, 68,
		156, 90,
		172, 90,
		180, 68,
		188, 68,
		197, 91,
		214, 91,
		220, 68,
		233, 68,
		240, 91,
		256, 91,
		253, 68
	};
	pops_triangle.add(App->physics->CreateChain(702, 159, Tri1, 65, b2_staticBody));

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
	pops_triangle.add(App->physics->CreateChain(985, 480, Tri2, 17, b2_staticBody));

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
	pops_triangle.add(App->physics->CreateChain(627, 710, Tri3, 17, b2_staticBody));

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
		87, 36,
		77, 44,
		82, 66,
		69, 77,
		48, 68,
		39, 76,
		43, 99,
		32, 108
	};
	pops_square.add(App->physics->CreateChain(536, 445, Sqr, 33, b2_staticBody));

	//Flipper --------------------------
	
	
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
	//if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//{
	//	circles.add(App->physics->CreateCircle(1150, 800, 16, b2_dynamicBody));
	//	circles.getLast()->data->listener = this;
	//}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		//boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50, b2_dynamicBody));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		circles.clear();
	}

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
		//b2Vec2 position(1150.0f, 800.0f);
		
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			b2Vec2 position(23.1f, 16.9f);
			c->data->body->SetTransform(position, 0);
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			b2Vec2 positionm(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
			c->data->body->SetTransform(positionm, 0);
			c->data->body->IsActive();
		}
		App->renderer->Blit(circle, x, y, NULL, 2.0f, c->data->GetRotation());
		c = c->next;
	}

	c = sensors.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if (c->data->body->IsAwake() == false)
		{
			App->renderer->Blit(texture_sensor, x, y, NULL, 2.0f);
		}
		if (c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()) && c->data->body->IsAwake())
		{
			
			c->data->body->SetAwake(false);
		
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

	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 200, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}
}
