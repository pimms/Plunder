#pragma once
#include "Loot.h"

using namespace Pim;

class LootBag : public Loot
{
public:
	LootBag(Vec2 pos);
	~LootBag();

	void update(float dt);

	bool hasLoot();
	void pickUp();

private:
	Sprite	*guide;
};

