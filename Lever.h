#pragma once
#include "Entity.h"

using namespace Pim;

class Door;

class Lever : public Entity
{
public:
	Lever(Vec2 pos);
	~Lever(void);
	
	void update(float dt);

	bool canUseLever();
	void useLever();

	void actionCompleted(BaseAction*);

	Door	*door;

private:
	Sprite	*handle;
	Sprite	*guide;

	b2Body	*sensor;
	
	bool	used;
	RotateByAction *rotate;
};

