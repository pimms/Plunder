#include "Entity.h"
#include "GameLayer.h"
#include "GameScene.h"

using namespace Pim;

/* Static */
b2Body* Entity::otherColBody(b2Body *myBody, uint16 cat)
{
	// Scan for touching levers
	for (auto ce=myBody->GetContactList(); ce; ce=ce->next)
	{
		b2Contact *c = ce->contact;
		if (c->IsTouching())
		{
			b2Fixture *oFix = NULL;
		
			if (c->GetFixtureA() == myBody->GetFixtureList())
			{
				oFix = c->GetFixtureB();
			}
			else
			{
				oFix = c->GetFixtureA();
			}

			if (oFix->GetFilterData().categoryBits == cat)
			{
				return oFix->GetBody();
			}
		}
	}

	return NULL;
}

Entity::Entity(void)
{
	body = NULL;

	useBatchNode(GameScene::getActorSheet());
}
Entity::~Entity(void)
{
	if (body)
	{
		b2World *world = GameLayer::getWorld();
		if (world)
		{
			world->DestroyBody(body);
		}
	}
}

b2Body* Entity::createRectangularBody(Vec2 pixelDim, uint16 category, uint16 maskBits)
{
	b2PolygonShape shape;
	shape.SetAsBox(pixelDim.x/(PTMR*2.f), pixelDim.y/(PTMR*2.f));

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.userData				= this;
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= maskBits;
	
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = toB2(position);
	bd.fixedRotation = true;
	bd.allowSleep = false;
	bd.userData = this;

	b2Body *retBody = GameLayer::getWorld()->CreateBody(&bd);
	retBody->CreateFixture(&fd);

	return retBody;
}
b2Body* Entity::createCircularBody(float pixRad, uint16 cat, uint16 mask)
{
	b2CircleShape shape;
	shape.m_radius = pixRad / PTMR;

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.userData				= this;
	fd.filter.categoryBits	= cat;
	fd.filter.maskBits		= mask;
	
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position = toB2(position);
	bd.fixedRotation = true;
	bd.allowSleep = false;
	bd.userData = this;

	b2Body *retBody = GameLayer::getWorld()->CreateBody(&bd);
	retBody->CreateFixture(&fd);

	return retBody;
}