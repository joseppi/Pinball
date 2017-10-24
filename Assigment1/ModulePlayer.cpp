#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		prismatic_joint->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
   		prismatic_joint->EnableMotor(false);
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		revolute_joint_left->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		revolute_joint_left->EnableMotor(false);
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::setSpring()
{
	spring = App->physics->CreateRectangle(1156, 800, 41, 100, b2_dynamicBody);
	spring_pivot = App->physics->CreateRectangle(1156, 980, 41, 30, b2_staticBody);

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
	PrismDef.maxMotorForce = 3000;
	
	prismatic_joint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&PrismDef);
}

void ModulePlayer::setLeftFlipper(int* flipper)
{
	//flipper_left = App->physics->CreateRectangle(839, 955, 100, 20, b2_staticBody);
	flipper_left = App->physics->CreateChain(780, 740, flipper, 45, b2_dynamicBody);
	flipper_pivot_left = App->physics->CreateCircle(800, 756, 10, b2_staticBody);

	b2RevoluteJointDef revDef;

	revDef.bodyA = flipper_left->body;
	revDef.bodyB = flipper_pivot_left->body;
	revDef.collideConnected = false;

	revDef.localAnchorA.Set(PIXEL_TO_METERS(23), PIXEL_TO_METERS(30));
	revDef.localAnchorB.Set(0, 0);

	revDef.referenceAngle = 90 * DEGTORAD;

	revDef.enableLimit = false;
	revDef.lowerAngle = 0;
	revDef.upperAngle = 30;

	revDef.enableMotor = false;
	revDef.motorSpeed = 3000 * DEGTORAD;
	revDef.maxMotorTorque = 1500;

	revolute_joint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revDef);
}