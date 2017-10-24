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

	return UPDATE_CONTINUE;
}

void ModulePlayer::setSpring()

{
	spring = App->physics->CreateRectangle(1156, 700, 41, 100, b2_dynamicBody);
	spring_pivot = App->physics->CreateRectangle(1156, 900, 41, 30, b2_staticBody);

	b2PrismaticJointDef def;

	def.bodyA = spring->body;
	def.bodyB = spring_pivot->body;

	def.localAnchorA.Set(0, 0);
	def.localAnchorB.Set(0, 0);

	def.collideConnected = true;
	def.localAxisA.Set(0, 1);

	def.enableLimit = true;
	def.upperTranslation = PIXEL_TO_METERS(100);
	def.lowerTranslation = 0;
	def.enableMotor = false;
	def.motorSpeed = 200;
	def.maxMotorForce = 5000;

	prismatic_joint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&def);
}

