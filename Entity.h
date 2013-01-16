#pragma once
#include "Plunder.h"

using namespace Pim;

class Entity : public Sprite
{
public:
	static b2Body* otherColBody(b2Body *myBody, uint16 oFixCategory);

	Entity(void);
	~Entity(void);

	b2Body* createRectangularBody(Vec2 pixelDim, uint16 category, uint16 maskBits);
	b2Body* createCircularBody(float pixelRad, uint16 categori, uint16 maskBits);

	Vec2				bodyOffset;
	b2Body				*body;
};

