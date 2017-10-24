#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void setSpring();
	void setLeftFlipper(int* flipper);
	void setRightFlipper();

public:


private:
	PhysBody* spring;
	PhysBody* spring_pivot;
	PhysBody* flipper_pivot_left;
	PhysBody* flipper_left;
	
	b2PrismaticJoint* prismatic_joint;
	b2RevoluteJoint* revolute_joint_left;
};