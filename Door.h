#pragma once
#include "entity.h"

using namespace Pim;

class Lever;

class Door : public Entity
{
public:
	Door(Vec2 pos, int direction, bool staticBody);
	~Door(void);

	void update(float);
	
	void open();
	void actionCompleted(BaseAction*);

private:
	TintAction *fadeout;

	bool		deleteNextFrame;
};

