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

	//tx_flipper_left = App->textures->Load("pinball/Flipper.png");
	//tx_flipper_right = App->textures->Load("pinball/Flipper.png");

	setSpring();
	setLeftFlipper();
	setRightFlipper();

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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		revolute_joint_left->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		revolute_joint_left->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		revolute_joint_right->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		revolute_joint_right->EnableMotor(false);
	}

	//Blitting flippers-------------------------------------------------
	//flipper_left->GetPosition(position.x, position.y);
	//App->renderer->Blit(tx_flipper_left, position.x, position.y, NULL, 1.0f, flipper_left->GetRotation());

	//flipper_right->GetPosition(position.x, position.y);
	//App->renderer->Blit(tx_flipper_right, position.x, position.y, NULL, 1.0f, flipper_right->GetRotation());

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

void ModulePlayer::setLeftFlipper() 
{
	flipper_left = App->physics->CreateRectangle(711, 965, 150, 30, b2_dynamicBody);
	flipper_pivot_left = App->physics->CreateCircle(711, 965, 10, b2_staticBody);

	b2RevoluteJointDef revDef;

	revDef.bodyA = flipper_left->body;
	revDef.bodyB = flipper_pivot_left->body;
	flipper_left->body->SetGravityScale(10.0f);

	revDef.localAnchorA.Set(PIXEL_TO_METERS(-55), 0);
	revDef.localAnchorB.Set(0, 0);
	revDef.collideConnected = false;

	revDef.enableLimit = true;
	revDef.upperAngle = 28 * DEGTORAD;
	revDef.lowerAngle = -28 * DEGTORAD;

	revDef.motorSpeed = 3000.0f * DEGTORAD;
	revDef.maxMotorTorque = 1500.0f;
	revDef.enableMotor = false;

	revolute_joint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revDef);
}

void ModulePlayer::setRightFlipper() 
{

	flipper_right = App->physics->CreateRectangle(999, 965, 150, 30, b2_dynamicBody);
	flipper_pivot_right = App->physics->CreateCircle(999, 965, 10, b2_staticBody);

	b2RevoluteJointDef revDef;

	revDef.bodyA = flipper_right->body;
	revDef.bodyB = flipper_pivot_right->body;
	flipper_right->body->SetGravityScale(10.0f);
	
	revDef.localAnchorA.Set(PIXEL_TO_METERS(55), 0);
	revDef.localAnchorB.Set(0, 0);
	revDef.collideConnected = false;

	revDef.enableLimit = true;
	revDef.upperAngle = 28 * DEGTORAD;
	revDef.lowerAngle = -28 * DEGTORAD;

	revDef.motorSpeed = -3000.0f * DEGTORAD;
	revDef.maxMotorTorque = 1500.0f;
	revDef.enableMotor = false;

	revolute_joint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revDef);
}