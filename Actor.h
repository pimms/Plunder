#pragma once
#include "Plunder.h"
#include "Entity.h"

using namespace Pim;

class Actor : public Entity
{
public:
	Actor();
	~Actor(void);

	virtual void update(float dt);

	virtual void prepareFrames()	= 0;
	virtual void createBody()		= 0;

protected:
	Sprite	*equip;

	int		direction;

	Rect	actorFrames[4];
	Rect	equipFrames[4];

	float	walkSpeed;
	Vec2	vel;
};

