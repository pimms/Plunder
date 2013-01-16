#include "Lever.h"
#include "GameScene.h"
#include "Door.h"

using namespace Pim;

Lever::Lever(Vec2 pos)
{
	listenFrame();

	position = pos;
	anchor	 = Vec2(0.5f, 0.f);
	setZOrder((int)position.y);

	rect	= Rect(0, 128, 58, 26);
	used	= false;
	rotate	= NULL;
	door	= NULL;

	handle = new Sprite;
	handle->useBatchNode(GameScene::getActorSheet());
	handle->anchor = Vec2(0.5f, 0.f);
	handle->rect = Rect(58, 128, 8, 44);
	handle->position = Vec2(0.f, 8.f);
	handle->rotation = 60.f;
	addChild(handle);

	guide = new Sprite;
	guide->useBatchNode(GameScene::getActorSheet());
	guide->rect = Rect(100, 0, 27, 27);
	guide->position = Vec2(0.f, 62.f);
	addChild(guide);

	body = createRectangularBody(Vec2(50.f, 10.f), CF_WALLS, ~0);
	body->SetType(b2_staticBody);
	
	sensor = createRectangularBody(Vec2(50.f, 25.f), CF_LEVER, ~0);
	sensor->GetFixtureList()->SetSensor(true);
	sensor->SetType(b2_staticBody);
}
Lever::~Lever(void)
{
}

void Lever::update(float dt)
{
	guide->hidden = !(!used && otherColBody(sensor, CF_PLAYER));
}

bool Lever::canUseLever()
{
	return !used;
}
void Lever::useLever()
{
	if (!used)
	{
		used = true;

		guide->hidden = true;

		rotate = new RotateByAction(-120.f, 1.f);
		rotate->notifyOnCompletion = true;
		rotate->notificationCallback = this;
		handle->runAction(rotate);
	}
}

void Lever::actionCompleted(BaseAction *act)
{
	if (act == rotate)
	{
		if (door)
		{
			door->open();
		}
	}
}