#include "Door.h"
#include "Lever.h"
#include "GameLayer.h"

using namespace Pim;

Door::Door(Vec2 pos, int direction, bool staticBody)
{
	listenFrame();

	position = pos;
	setZOrder((int)position.y);

	rect = Rect(69, 145, 145, 31);
	anchor = Vec2(0.5f, 0.5f);
	bodyOffset = Vec2(145.f, 0.f);

	deleteNextFrame = false;

	if (direction == D_UP)
	{
		rotation = 90.f;
	}
	else if (direction == D_RIGHT)
	{
		rotation = 180.f;
	}
	else if (direction == D_DOWN)
	{
		rotation = 270.f;
	}
	else if (direction == D_LEFT)
	{
		rotation = 0.f;
	}

	/* Physics */
	body = createRectangularBody(Vec2(143.f, 11.f), CF_WALLS, ~0);
	body->SetTransform(body->GetPosition(), rotation*DEGTORAD);
	 
	if (staticBody)
	{
		body->SetType(b2_staticBody);
	}

	/* Shadows */
	Vec2 verts[] = {
		Vec2(-72.f, -14.5f),
		Vec2( 72.f, -14.5f),
		Vec2( 72.f,  14.5f),
		Vec2(-72.f,  14.5f),
	};

	setShadowShape(verts, 4);
	GameLayer::getSingleton()->addShadowCaster(this);
}
Door::~Door(void)
{
}

void Door::update(float dt)
{
	if (deleteNextFrame)
	{
		getParent()->removeChild(this);
	}
}

void Door::open()
{
	fadeout = new TintAction(Color(1.f, 1.f, 1.f, 0.f), 1.f);
	fadeout->notifyOnCompletion = true;
	runAction(fadeout);
}
void Door::actionCompleted(BaseAction *act)
{
	if (act == fadeout)
	{
		deleteNextFrame = true;
	}
}