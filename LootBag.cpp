#include "LootBag.h"
#include "GameScene.h"

LootBag::LootBag(Vec2 pos)
{
	useBatchNode(GameScene::getActorSheet());

	position = pos;
	scale = Vec2(1.5f, 1.5f);
	rect = Rect(0, 98, 18, 24);

	setZOrder((int)position.y);

	// Create a sensor body (no collision body)
	b2PolygonShape shape;
	shape.SetAsBox(0.55f, 0.5f);

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.isSensor				= true;
	fd.filter.categoryBits	= CF_LOOT;
	fd.filter.maskBits		= ~0;
	fd.userData = this;

	b2BodyDef bd;
	bd.position = toB2(position);
	bd.userData = this;

	body = GameLayer::getWorld()->CreateBody(&bd);
	body->CreateFixture(&fd);

	guide = new Sprite;
	guide->useBatchNode(GameScene::getActorSheet());
	guide->rect = Rect(100, 0, 27, 27);
	guide->position = Vec2(0.f, 30.f);
	addChild(guide);

	listenFrame();
}
LootBag::~LootBag()
{
}

void LootBag::update(float dt)
{
	guide->hidden = !otherColBody(body, CF_PLAYER);
}

bool LootBag::hasLoot()
{
	return true;
}
void LootBag::pickUp()
{
	if (getParent())
	{
		getParent()->removeChild(this);
	}
}
