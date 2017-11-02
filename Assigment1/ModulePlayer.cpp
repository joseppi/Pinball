#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include <ctime>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	tx_flipper_left = nullptr;
	tx_flipper_right = nullptr;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	tx_spring = App->textures->Load("pinball/Spring.png");
	game_over = App->textures->Load("pinball/game_over.png");

	tx_flipper_left = App->textures->Load("pinball/left_Flipper.png");
	tx_flipper_right = App->textures->Load("pinball/right_Flipper.png");

	setSpring();
	setLeftFlipper();
	setRightFlipper();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(tx_spring);

	App->textures->Unload(tx_flipper_left);
	App->textures->Unload(tx_flipper_left);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//SPRING
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->scene_intro->lives > 0)
	{
		prismatic_joint->EnableMotor(true);
		App->audio->PlayFx(App->scene_intro->spring_fx);

	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
   		prismatic_joint->EnableMotor(false);
	}
	srand(time(NULL));
	prismatic_joint->SetMaxMotorForce(3050 + rand() % 1000);

	//LEFT FLIPPER
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && App->scene_intro->lives > 0)
	{
		revolute_joint_left->EnableMotor(true);
		App->audio->PlayFx(App->scene_intro->flipper_fx);
		App->scene_intro->pops_mini.getFirst()->data->body->SetTransform({ PIXEL_TO_METERS(42), PIXEL_TO_METERS(670) }, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		revolute_joint_left->EnableMotor(false);
		App->scene_intro->pops_mini.getFirst()->data->body->SetTransform({ PIXEL_TO_METERS(42), PIXEL_TO_METERS(1670) }, 0);
	}
	//RIGHT FLIPPER
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && App->scene_intro->lives > 0)
	{
		revolute_joint_right->EnableMotor(true);
		App->audio->PlayFx(App->scene_intro->flipper_fx);
		App->scene_intro->pops_mini.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(581), PIXEL_TO_METERS(703) }, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		revolute_joint_right->EnableMotor(false);
		App->scene_intro->pops_mini.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(581), PIXEL_TO_METERS(1703) }, 0);
	}

	//Blitting flippers-----------------------------------------------
	flipper_left->GetPosition(position.x, position.y);
	App->renderer->Blit(tx_flipper_left, position.x - 10, position.y - 7, NULL, 28.0f,  flipper_left->GetRotation());

	flipper_right->GetPosition(position.x, position.y);
	App->renderer->Blit(tx_flipper_right, position.x - 6, position.y - 9, NULL, 28.0f,  flipper_right->GetRotation());

	//Blitting spring-------------------------------------------------
	spring->GetPosition(position.x, position.y);
	App->renderer->Blit(tx_spring, position.x, position.y - 1, NULL, 1.0f);

	//Blitting gameover------------------------------------------------
	if (App->scene_intro->lives == 0)
	{
		App->renderer->Blit(game_over, 0, 0, NULL, 1.0f);
	}
	

	return UPDATE_CONTINUE;
}

void ModulePlayer::setSpring()
{
	srand(time(NULL));

	spring = App->physics->CreateRectangle(644, 745, 41, 100, 0.0f, b2_dynamicBody);
	spring_pivot = App->physics->CreateRectangle(644, 925, 41, 30, 0.0f, b2_staticBody);

	b2PrismaticJointDef PrismDef;

	PrismDef.bodyA = spring->body;
	PrismDef.bodyB = spring_pivot->body;

	PrismDef.localAnchorA.Set(0, 0);
	PrismDef.localAnchorB.Set(0, 0);

	PrismDef.collideConnected = true;
	PrismDef.localAxisA.Set(0, 1);
	
	PrismDef.enableLimit = true;
	PrismDef.upperTranslation = PIXEL_TO_METERS(100);
	PrismDef.lowerTranslation = 0;
	PrismDef.enableMotor = false;
	PrismDef.motorSpeed = 50;
	PrismDef.maxMotorForce = 2750 + rand() % 1000;//3750
	
	prismatic_joint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&PrismDef);
}

void ModulePlayer::setLeftFlipper() 
{
	flipper_left = App->physics->CreateRectangle(199, 908, 150, 30, 0.0f, b2_dynamicBody);
	flipper_pivot_left = App->physics->CreateCircle(199, 908, 10, 0.0f, b2_staticBody);

	b2RevoluteJointDef revDef;

	revDef.bodyA = flipper_left->body;
	revDef.bodyB = flipper_pivot_left->body;
	flipper_left->body->SetGravityScale(10.0f);

	revDef.localAnchorA.Set(PIXEL_TO_METERS(-55), 0);
	revDef.localAnchorB.Set(0, 0);
	revDef.collideConnected = false;

	revDef.enableLimit = true;
	revDef.upperAngle = 28 * DEGTORAD;
	revDef.lowerAngle = -26 * DEGTORAD;

	revDef.motorSpeed = 5000.0f * DEGTORAD;
	revDef.maxMotorTorque = 1750.0f;
	revDef.enableMotor = false;

	revolute_joint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revDef);
}

void ModulePlayer::setRightFlipper() 
{
	flipper_right = App->physics->CreateRectangle(487, 908, 150, 30, 0.0f, b2_dynamicBody);
	flipper_pivot_right = App->physics->CreateCircle(487, 908, 10, 0.0f, b2_staticBody);

	b2RevoluteJointDef revDef;

	revDef.bodyA = flipper_right->body;
	revDef.bodyB = flipper_pivot_right->body;
	flipper_right->body->SetGravityScale(10.0f);
	
	revDef.localAnchorA.Set(PIXEL_TO_METERS(55), 0);
	revDef.localAnchorB.Set(0, 0);
	revDef.collideConnected = false;

	revDef.enableLimit = true;
	revDef.upperAngle = 26 * DEGTORAD;
	revDef.lowerAngle = -28 * DEGTORAD;

	revDef.motorSpeed = -5000.0f * DEGTORAD;
	revDef.maxMotorTorque = 1750.0f;
	revDef.enableMotor = false;

	revolute_joint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revDef);
}

void ModulePlayer::setSensorCirclesRed(PhysBody* sensor, SDL_Texture* texture, bool active, int *activeSensors, bool *resetSensor)
{

}

void ModulePlayer::setSensorCircles(PhysBody* sensor, SDL_Texture* texture, bool active)
{
	App->scene_intro->b = sensor;

	if (App->scene_intro->b != NULL)
	{
		int x, y;
		App->scene_intro->b->GetPosition(x, y);
		if (App->scene_intro->b->body->IsAwake() == false)
		{
			App->renderer->Blit(texture, x, y, NULL, 2.0f);
		}
		if (active == true && App->scene_intro->b->body->IsAwake() == true)
		{
			App->scene_intro->b->body->SetAwake(false);
			App->audio->PlayFx(App->scene_intro->bouncers_fx);
		}
	}
}