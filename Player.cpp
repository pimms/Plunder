#include "Player.h"
#include "GameScene.h"
#include "GameLayer.h"
#include "Chest.h"
#include "LootBag.h"
#include "Lever.h"
#include "Underground.h"

using namespace Pim;

#define SPEED_NORM		350.f
#define SPEED_SHRINK	200.f

Player::Player()
{
	rect		 = Rect(0.f, 0.f, 35.f, 52.f);
	position	 = Vec2(3304.f, 2155.f);
	anchor		 = Vec2(0.5f, 0.25f);

	blurIdx		 = 0;

	sprintReqRelease = false;
	keyEvt		 = NULL;
	conEvt		 = NULL;
	sprint		 = 1.f;
	walkSpeed	 = SPEED_NORM;
	shrink		 = false;
	sizeTransit  = false;
	carryingLoot = false;

	fadeIn		 = NULL;
	fadeOut		 = NULL;

	prepareFrames();
	createBody();

	equip = new Sprite;
	equip->useBatchNode(GameScene::getActorSheet());
	equip->anchor = Vec2(0.f, 0.f);
	equip->hidden = true;
	addChild(equip);

	hat = new Sprite;
	hat->useBatchNode(GameScene::getActorSheet());
	hat->rect = Rect(50, 37, 43, 14);
	hat->hidden = true;
	hat->position = Vec2(0.f, 30.f);
	addChild(hat);

	listenInput();
	listenController();
	listenFrame();
}
Player::~Player()
{
}

void Player::prepareFrames()
{
	actorFrames[D_DOWN] = Rect(0.f, 0.f, 35.f, 52.f);
	actorFrames[D_UP] = actorFrames[D_DOWN];
	actorFrames[D_RIGHT] = actorFrames[D_DOWN];
	actorFrames[D_LEFT] = actorFrames[D_DOWN];

	equipFrames[D_DOWN] = Rect(18, 98, 18, 29);
	equipFrames[D_UP] = equipFrames[D_DOWN];
	equipFrames[D_RIGHT] = equipFrames[D_DOWN];
	equipFrames[D_LEFT] = equipFrames[D_DOWN];
}
void Player::createBody()
{
	body = Actor::createRectangularBody(
		Vec2(33.f, 25.f), 
		CF_PLAYER, 
		CF_WALLS | CF_INTERACTIVE | CF_GUARD 
	);
	body->SetTransform(toB2(position), 0.f);

	Vec2 verts[4] = {
		Vec2(-33.f*0.5f*scale.x, -52.f*0.25f*scale.y),
		Vec2( 33.f*0.5f*scale.x, -52.f*0.25f*scale.y),
		Vec2( 33.f*0.5f*scale.x,  52.f*0.75f*scale.y),
		Vec2(-33.f*0.5f*scale.x,  52.f*0.75f*scale.y),
	};

	setShadowShape(verts, 4);
	GameLayer::getSingleton()->addShadowCaster(this);
}

void Player::draw()
{
	float curAlpha = color.a;

	for (int i=blurIdx; i>=0; i--)
	{
		if (i != 0)
		{
			color.a = (10.f-i)/25.f;
		}
		else
		{
			color.a = curAlpha;
		}

		equip->color.a = color.a;
		hat->color.a = color.a;

		hat->scale = scale;
		hat->position.y = 39.f * scale.y;

		position = blurPos[i];
		Sprite::draw();
	}

	/*
	glPushMatrix();		// <NODE AND CHILDREN>

	Vec2 cfac = GameControl::getSingleton()->coordinateFactor();

	glTranslatef(position.x / cfac.x, position.y / cfac.y, 0.f);

	glRotatef(rotation, 0.f, 0.f, 1.f);

	glPushMatrix();		// <NODE>

	Vec2 fac = GameControl::getSingleton()->windowScale();
	glScalef(scale.x * fac.x, scale.y * fac.y, 1.f);
	glColor4f(color.r, color.g, color.b, color.a);

	glBindTexture(GL_TEXTURE_2D, texID);

	for (int i=0; i<10; i++)
	{
		glBegin(GL_QUADS);
			glTexCoord2f((float)rect.x / (float)_tw, (float)rect.y / (float)_th);
			glVertex2f(-anchor.x * rect.width, -anchor.y * rect.height);

			glTexCoord2f((float)rect.x/(float)_tw + (float)rect.width/(float)_tw, (float)rect.y / (float)_th);
			glVertex2f((1.f-anchor.x) * rect.width, -anchor.y * rect.height);

			glTexCoord2f((float)rect.x/(float)_tw + (float)rect.width/(float)_tw, (float)rect.y/(float)_th + (float)rect.height/(float)_th);
			glVertex2f((1.f-anchor.x) * rect.width, (1.f-anchor.y) * rect.height);

			glTexCoord2f((float)rect.x/(float)_tw, (float)rect.y/(float)_th + (float)rect.height/(float)_th);
			glVertex2f(-anchor.x * rect.width, (1.f-anchor.y) * rect.height);
		glEnd();
	}

	glPopMatrix();		// </NODE>

	orderChildren();
	for (unsigned int i=0; i<children.size(); i++)
	{
		children[i]->draw();
	}

	glPopMatrix();		// </NODE AND CHILDREN>
	*/
}

void Player::update(float dt)
{
	Actor::update(dt);

	float sprFac = sprintFactor(dt);

	body->SetLinearVelocity(toB2(vel * walkSpeed * sprFac));
	setZOrder((int)position.y);

	if (sizeTransit)
	{
		GameLayer::getSingleton()->removeShadowCaster(this);

		Vec2 verts[4] = {
			Vec2(-33.f*0.5f*scale.x, -52.f*0.25f*scale.y),
			Vec2( 33.f*0.5f*scale.x, -52.f*0.25f*scale.y),
			Vec2( 33.f*0.5f*scale.x,  52.f*0.75f*scale.y),
			Vec2(-33.f*0.5f*scale.x,  52.f*0.75f*scale.y),
		};

		setShadowShape(verts, 4);
		GameLayer::getSingleton()->addShadowCaster(this);
	}
}

bool Player::sprintKeyDown()
{
	bool shouldSprint = false;

	if (keyEvt)
	{
		if (keyEvt->isKeyDown(KeyEvent::K_SHIFT))
		{
			shouldSprint = true;
		}
	}

	if (conEvt)
	{
		if (conEvt->leftTrigger() > 0.5f || conEvt->rightTrigger() > 0.5f)
		{
			shouldSprint = true;
		}
	}

	if (sprintReqRelease)
	{
		if (!shouldSprint)
		{
			sprintReqRelease = false;
		}
		return false;
	}
	
	return shouldSprint;
}
float Player::sprintFactor(float dt)
{
	float fac = 1.0f;

	/* Order the blur positions */
	for (int i=9; i>=1; i--)
	{
		blurPos[i] = blurPos[i-1];
	}

	if (sprintKeyDown())
	{
		fac = 1.4f;
		sprint -= dt*2.f;

		blurIdx = 9;

		if (sprint < 0.f)
		{
			sprintReqRelease = true;
		}

		blurPos[0] = position;
	}
	else
	{
		if (blurIdx > 1)
		{
			blurIdx--;
		}

		sprint += dt;
		
		if (sprint >= 1.f)
		{
			sprint = 1.f;
		}

		blurPos[blurIdx] = Vec2(-100.f, -100.f);
		blurPos[0] = position;
	}

	return fac;
}

void Player::keyEvent(KeyEvent &evt)
{
	keyEvt = &evt;

	if (evt.isKeyFresh(KeyEvent::K_L))
	{
		GameScene::getHUDLayer()->displayLootSplash();
	}

	// Shrink
	if (!sizeTransit && evt.isKeyFresh(KeyEvent::K_E))
	{
		setShrunk(!shrink);
	}

	// Action key
	if (evt.isKeyFresh(KeyEvent::K_F))
	{
		if (carryingLoot)
		{
			BanditChest *bc = nearbyBanditChest();
			if (bc)
			{
				carryingLoot = false;
				equip->hidden = true;

				GameLayer::lootAddedToChest();
			}
			else
			{
				dropLoot();
			}
		}
		else
		{
			Loot *loot = nearbyLoot();
			if (loot)
			{
				pickUpLoot(loot);
			}
			else
			{
				Lever *lever = nearbyLever();
				if (lever)
				{
					useLever(lever);
				}
				else
				{
					useNearbyUnderground();
				}
			}
		}
	}

	// Y axis movement
	if (evt.isKeyDown(KeyEvent::K_W))
	{
		vel.y = 1.f;
	}
	else if (evt.isKeyDown(KeyEvent::K_S))
	{
		vel.y = -1.f;
	}
	else
	{
		vel.y = 0.f;
	}

	// X axis movement
	if (evt.isKeyDown(KeyEvent::K_A))
	{
		vel.x = -1.f;
	}
	else if (evt.isKeyDown(KeyEvent::K_D))
	{
		vel.x = 1.f;
	}
	else
	{
		vel.x = 0.f;
	}

	if (vel.x != 0.f && vel.y != 0.f)
	{
		vel /= vel.length();
	}
}
void Player::controllerEvent(ControllerEvent &evt)
{
	conEvt = &evt;
	// Shrink
	if (!sizeTransit && evt.isKeyFresh(ControllerEvent::X_X))
	{
		setShrunk(!shrink);
	}

	// Action key
	if (evt.isKeyFresh(ControllerEvent::X_A))
	{
		if (carryingLoot)
		{
			BanditChest *bc = nearbyBanditChest();
			if (bc)
			{
				carryingLoot = false;
				equip->hidden = true;

				GameLayer::lootAddedToChest();
			}
			else
			{
				dropLoot();
			}
		}
		else
		{
			Loot *loot = nearbyLoot();
			if (loot)
			{
				pickUpLoot(loot);
			}
			else
			{
				Lever *lever = nearbyLever();
				if (lever)
				{
					useLever(lever);
				}
				else
				{
					useNearbyUnderground();
				}
			}
		}
	}

	vel = evt.leftStick();
}

void Player::setShrunk(bool mini)
{
	sizeTransit = true;

	hat->hidden = !mini;

	if (mini)
	{
		dropLoot();

		scaleAction = new ScaleToAction(Vec2(0.25f, 0.25f), 0.5f);
		scaleAction->notifyOnCompletion = true;
		runAction(scaleAction);

		GameLayer::scaleTo(1.5f);
		GameLayer::getWorld()->DestroyBody(body);
		body = createRectangularBody(
			Vec2(0.25f,0.25f) * Vec2(33.f, 25.f),
			CF_PLAYER, 
			CF_WALLS | CF_INTERACTIVE | CF_GUARD 
		);
	}
	else
	{
		scaleAction = new ScaleToAction(Vec2(1.f, 1.f), 0.5f);
		scaleAction->notifyOnCompletion = true;
		runAction(scaleAction);

		GameLayer::scaleTo(1.f);
		GameLayer::getWorld()->DestroyBody(body);
		body = Actor::createRectangularBody(
			Vec2(33.f, 25.f), 
			CF_PLAYER, 
			CF_WALLS | CF_INTERACTIVE | CF_GUARD 
		);
	}
}
void Player::actionCompleted(BaseAction *action)
{
	if (action == scaleAction)
	{
		scaleAction = NULL;
		sizeTransit = false;
		shrink		= !shrink;

		if (shrink)
		{
			walkSpeed = SPEED_SHRINK;
		}
		else
		{
			walkSpeed = SPEED_NORM;
		}

		/*
		GameLayer::getSingleton()->removeShadowCaster(this);

		Vec2 verts[4] = {
			Vec2(-33.f*0.5f*scale.x, -52.f*0.25f*scale.y),
			Vec2( 33.f*0.5f*scale.x, -52.f*0.25f*scale.y),
			Vec2( 33.f*0.5f*scale.x,  52.f*0.75f*scale.y),
			Vec2(-33.f*0.5f*scale.x,  52.f*0.75f*scale.y),
		};

		setShadowShape(verts, 4);
		GameLayer::getSingleton()->addShadowCaster(this);
		*/
	}
	else if (action == fadeOut)
	{
		body->SetTransform(toB2(ugDest), 0.f);
	}
	else if (action == fadeIn)
	{
		fadeOut = NULL;
		fadeIn = NULL;
	}
}

Loot* Player::nearbyLoot()
{
	// Scan for touching loot
	b2Body *lootBody = Entity::otherColBody(body, CF_LOOT);

	if (lootBody)
	{
		Loot *loot = static_cast<Loot*>(lootBody->GetUserData());
		if (loot)
		{
			if (abs((loot->position-position).length()) < 320.f)
			{
				return loot;
			}
		}
	}

	return NULL;
}
void Player::pickUpLoot(Loot *loot)
{
	if (loot && !shrink)
	{
		if (carryingLoot)
		{
			dropLoot();
		}

		if (dynamic_cast<LootBag*>(loot))
		{
			GameLayer::registerLootBag(LOOTREF_REMOVE, dynamic_cast<LootBag*>(loot));
		}

		loot->pickUp();
		carryingLoot = true;

		equip->hidden = false;
		equip->position = Vec2(14.5f, -13.f);

		GameLayer::lootDeliveryCheck();
	}
}
void Player::dropLoot()
{
	if (carryingLoot)
	{
		carryingLoot = false;
		equip->hidden = true;

		LootBag *bag = new LootBag(position + Vec2(0.f, 0.f));
		getParent()->addChild(bag);

		GameLayer::registerLootBag(LOOTREF_ADD, bag);
		GameLayer::lootDeliveryCheck();
	}
}

Lever* Player::nearbyLever()
{
	// Scan for touching levers
	b2Body *leverBody = Entity:: otherColBody(body, CF_LEVER);

	if (leverBody)
	{
		Lever *lever = static_cast<Lever*>(leverBody->GetUserData());
		if (lever)
		{
			return lever;
		}
	}

	return NULL;
}
void Player::useLever(Lever *lever)
{
	if (lever && !shrink)
	{
		lever->useLever();
	}
}

BanditChest* Player::nearbyBanditChest()
{
	b2Body *bcBody = Entity::otherColBody(body, CF_BANDITCHEST);

	if (bcBody)
	{
		BanditChest *bChest = static_cast<BanditChest*>(bcBody->GetUserData());
		if (bChest)
		{
			return bChest;
		}
	}

	return NULL;
}

void Player::useNearbyUnderground()
{
	if (shrink)
	{
		b2Body *ugBody = Entity::otherColBody(body, CF_UNDERGROUND);
	
		if (ugBody)
		{
			Underground *ug = static_cast<Underground*>(ugBody->GetUserData());
			if (ug)
			{
				int idx = ug->sensorIndex(ugBody);
				
				if (idx != 0)
				{
					fadeOut = new TintAction(Color(1.f, 1.f, 1.f, 0.f), 0.5f);
					fadeIn = new TintAction(Color(1.f, 1.f, 1.f, 1.f), 0.5f);
					fadeOut->notifyOnCompletion = true;
					fadeIn->notifyOnCompletion = true;

					ActionQueue *aq = new ActionQueue(2, fadeOut, fadeIn);
					runActionQueue(aq);

					ugDest = ug->getOtherIndexPos(idx);
				}
			}
		}
	}
}