#include "Underground.h"
#include "GameScene.h"

Underground::Underground(Vec2 p1, Vec2 p2)
{
	pos1 = p1;
	pos2 = p2;

	sensor1 = createRectangularBody(Vec2(25.f, 15.f), CF_UNDERGROUND, ~0);
	sensor1->GetFixtureList()->SetSensor(true);
	sensor1->SetTransform(toB2(pos1), 0.f);

	sensor2 = createRectangularBody(Vec2(25.f, 15.f), CF_UNDERGROUND, ~0);
	sensor2->GetFixtureList()->SetSensor(true);
	sensor2->SetTransform(toB2(pos2), 0.f);

	guide1 = new Sprite;
	guide1->useBatchNode(GameScene::getActorSheet());
	guide1->rect = Rect(100, 0, 27, 27);
	guide1->position = pos1 + Vec2(0.f, 50.f);
	guide1->setZOrder(guide1->position.y + 100);
	addChild(guide1);

	guide2 = new Sprite;
	guide2->useBatchNode(GameScene::getActorSheet());
	guide2->rect = Rect(100, 0, 27, 27);
	guide2->position = pos2 + Vec2(0.f, 50.f);
	guide2->setZOrder(guide2->position.y + 100);
	addChild(guide2);

	listenFrame();
}
Underground::~Underground(void)
{

}

void Underground::update(float dt)
{
	guide1->hidden = !otherColBody(sensor1, CF_PLAYER);
	guide2->hidden = !otherColBody(sensor2, CF_PLAYER);
}

unsigned int Underground::sensorIndex(b2Body *sensor)
{
	if (sensor == sensor1)
	{
		return 1;
	}
	else if (sensor == sensor2)
	{
		return 2;
	}

	return 0;
}
Vec2 Underground::getOtherIndexPos(unsigned int idx)
{
	if (idx == 1)
	{
		return pos2;
	}
	else if (idx == 2)
	{
		return pos1;
	}

	return Vec2(0.f, 0.f);
}
