#pragma once
#include "entity.h"

using namespace Pim;

class Underground : public Entity
{
public:
	Underground(Vec2 pos1, Vec2 pos2);
	~Underground(void);

	void update(float dt);

	unsigned int sensorIndex(b2Body*);		// Returns 1 or 2
	Vec2 getOtherIndexPos(unsigned int p);	// p=1 returns 2, p=2 returns 1

private:
	Sprite  *guide1;
	Sprite	*guide2;

	b2Body	*sensor1;
	b2Body	*sensor2;
	
	Vec2	pos1;
	Vec2	pos2;
};

