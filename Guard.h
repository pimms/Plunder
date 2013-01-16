#pragma once
#include "actor.h"
#include "PathNodeManager.h"
#include "GuardHelperObjects.h"

using namespace Pim;

class LootBag;

class Guard : public Actor
{
public:
	static deque<Waypoint*> pathIndex(const int idx);

	Guard(int pathIdx);
	~Guard(void);

	void prepareFrames();
	void createBody();
	
	void update(float dt);

	void moveTo(Waypoint *wp);
	void moveToPlayer();
	void moveToLoot(LootBag *loot);
	void moveToLKP();

	bool playerLos(float dt);
	bool waypointLos(Waypoint *wp);
	LootBag* lootLos(float dt);

	void reboundToPath();

private:
	deque<Waypoint*>	patrolPath;
	deque<Waypoint*>	curPath;
	Waypoint			*curWp;

	Sprite				*exclamation;

	bool				waiting;
	LootBag				*waitLoot;
	float				waitTimer;

	float				playerLosTimer;
	float				lootLosTimer;

	bool				playerShrunkLastFrame;
	bool				confused;			// When the player is tracked and shrinks
	float				confuseTimer;	

	bool				trackingPlayer;
	Vec2				playerLKP;

	Vec2				lframePos;
};