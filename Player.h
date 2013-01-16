#pragma once
#include "actor.h"

using namespace Pim;

class BanditChest;
class Underground;
class LootBag;
class Chest;
class Loot;

class Lever;

class Player : public Actor
{
public:
	Player();
	~Player();

	void prepareFrames();
	void createBody();

	void draw();

	void update(float dt);

	bool sprintKeyDown();
	float sprintFactor(float dt);

	void keyEvent(KeyEvent &evt);
	void controllerEvent(ControllerEvent &evt);

	bool getShrunk() { return shrink; }
	void setShrunk(bool shrunk);
	void actionCompleted(BaseAction *action);
	
	Loot* nearbyLoot();
	void pickUpLoot(Loot *loot);
	void dropLoot();

	Lever* nearbyLever();
	void useLever(Lever *lever);

	BanditChest *nearbyBanditChest();

	void useNearbyUnderground();	// Finds and uses nearby undergrounds

private:
	Sprite				*hat;

	bool				carryingLoot;

	KeyEvent			*keyEvt;
	ControllerEvent		*conEvt;
	float				sprint;

	unsigned int		blurIdx;
	Vec2				blurPos[10];
	bool				sprintReqRelease;
	
	SpriteAction		*scaleAction;
	bool				sizeTransit;
	bool				shrink;

	// Action-handles used when travelling underground
	TintAction			*fadeOut;
	TintAction			*fadeIn;
	Vec2				ugDest;
};

