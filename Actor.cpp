#include "Actor.h"
#include "GameScene.h"
#include "GameLayer.h"

using namespace Pim;

Actor::Actor()
{
	equip = NULL;
	direction = 0;
}
Actor::~Actor(void)
{
}

void Actor::update(float dt)
{
	if (vel.x < 0.f)
	{
		direction = D_LEFT;
	}
	else if (vel.x > 0.f)
	{
		direction = D_RIGHT;
	}
	else if (vel.y < 0.f)
	{
		direction = D_DOWN;
	}
	else if (vel.y > 0.f)
	{
		direction = D_UP;
	}

	rect = actorFrames[direction];
	
	if (equip)
	{
		equip->rect = equipFrames[direction];
	}
}