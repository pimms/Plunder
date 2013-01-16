#pragma once
#include "entity.h"
class BanditChest :
	public Entity
{
public:
	BanditChest(Vec2 pos);
	~BanditChest(void);

	void update(float dt);

private:
	Sprite	*guide;

	b2Body	*body;
	b2Body	*sensor;
};

