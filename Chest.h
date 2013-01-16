#pragma once
#include "Loot.h"

using namespace Pim;

class Chest : public Loot
{
public:
	Chest(Vec2 pos);
	~Chest();

	void update(float dt);

	bool hasLoot();
	void pickUp();

private:
	Sprite	*guide;

	b2Body	*body;
	b2Body	*sensor;

	bool	loot;
};

