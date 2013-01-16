#include "BanditChest.h"
#include "GameScene.h"

BanditChest::BanditChest(Vec2 pos)
{
	useBatchNode(GameScene::getActorSheet());
	rect	= Rect(65.f, 52.f, 63.f, 52.f);
	anchor	= Vec2(0.f, 0.f);
	position = pos;

	setZOrder((int)position.y);

	// Create body
	b2PolygonShape shape;
	shape.SetAsBox(61.f/(PTMR*2.f), 39.f/(PTMR*2.f), b2Vec2(1.f, 0.7f), 0.f);

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.userData				= this;
	fd.filter.categoryBits	= CF_WALLS;
	fd.filter.maskBits		= ~0;
	
	b2BodyDef bd;
	bd.type				= b2_staticBody;
	bd.position			= toB2(position);
	bd.fixedRotation	= true;
	bd.userData			= this;

	body = GameLayer::getWorld()->CreateBody(&bd);
	body->CreateFixture(&fd);

	// Create sensor
	shape.SetAsBox(50.f/(PTMR*2.f), 20.f/(PTMR*2.f), b2Vec2(1.f, 0.7f), 0.f);

	fd.shape				= &shape;
	fd.isSensor				= true;
	fd.userData				= this;
	fd.filter.categoryBits	= CF_BANDITCHEST;
	fd.filter.maskBits		= ~0;

	bd.position = toB2(position + Vec2(0.f, -20.f));

	sensor = GameLayer::getWorld()->CreateBody(&bd);
	sensor->CreateFixture(&fd);

	// Create guide sprite
	guide = new Sprite;
	guide->useBatchNode(GameScene::getActorSheet());
	guide->rect = Rect(100, 0, 27, 27);
	guide->position = Vec2(30.5f, 62.f);
	addChild(guide);

	listenFrame();
}
BanditChest::~BanditChest(void)
{
}

void BanditChest::update(float dt)
{
	guide->hidden = !sensor || !otherColBody(sensor, CF_PLAYER);
}