#pragma once
#include "Plunder.h"
#include "Entity.h"

class Loot : public Entity
{
public:
	virtual bool hasLoot() = 0;
	virtual void pickUp() = 0;
};

