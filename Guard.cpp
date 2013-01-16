#include "Guard.h"
#include "GameScene.h"
#include "GameLayer.h"
#include "Player.h"
#include "Player.h"
#include "GameLayer.h"
#include "LootBag.h"

#define SPEED_PATROL	200.f
#define SPEED_CHASE		400.f

#define LOSLEN_NORM		240.f
#define LOSLEN_SHRINK	140.f
#define LOSTIME			0.5f

/* STATIC */
template <typename T, size_t N>
inline size_t arrlen(const T(&)[N]) { return N; }
deque<Waypoint*> Guard::pathIndex(const int idx)
{
	deque<Waypoint*> path;

	if (idx == 0)
	{
		int indices[] = {33, 0, 1, 6, 5, 65, 5, 6, 1, 0};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 1)
	{
		int indices[] = {6, 7, 8, 9, 10, 9, 8, 7};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 2)
	{
		int indices[] = {12, 11, 10, 11};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 3)
	{
		int indices[] = {79, 80, 20, 21, 20, 80};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 4)
	{
		int indices[] = {2, 3, 31, 30, 31, 3};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 5)
	{
		int indices[] = {87, 86, 85, 32, 34, 32, 85, 86};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 6)
	{
		int indices[] = {18, 36, 83, 84, 92, 84, 83, 36};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 7)
	{
		int indices[] = {79, 81, 88, 89, 90, 89, 88, 81};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 8)
	{
		int indices[] = {82, 35, 27, 35};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 9)
	{
		int indices[] = {66, 24, 25, 28, 25, 24};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	/*
	else if (idx == 10)
	{
		int indices[] = {21, 22, 23, 24, 23, 22};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	else if (idx == 11)
	{
		int indices[] = {30, 26, 25, 69, 25, 26};
		for (int i=0; i<arrlen(indices); i++)
			path.push_back(GameLayer::getSingleton()->waypoints[indices[i]]);
	}
	*/
	else
	{
		printf("Could not find path for index %i - returning an empty path\n", idx);
	}

	return path;
}


Guard::Guard(int pathIdx)
{
	listenFrame();

	walkSpeed		= 250.f;
	anchor			= Vec2(0.5f, 0.25f);
	patrolPath		= pathIndex(pathIdx);
	curPath			= patrolPath;
	curWp			= NULL;

	waitLoot		= NULL;
	waiting			= false;
	waitTimer		= 0.f;
	trackingPlayer	= false;
	lootLosTimer	= 0.f;
	playerLosTimer	= 0.f;
	confused		= false;
	confuseTimer	= 0.f;
	playerShrunkLastFrame = false;

	if (patrolPath.size())
	{
		curWp = curPath.front();
		position = curWp->pos;
		curPath.pop_front();
	}

	prepareFrames();
	createBody();

	exclamation = new Sprite();
	exclamation->useBatchNode(GameScene::getActorSheet());
	exclamation->rect = Rect(146, 105, 13, 31);
	exclamation->anchor = Vec2(0.5f, 0.f);
	exclamation->position = Vec2(0.f, 41.f);
	exclamation->color.a = 0.f;
	addChild(exclamation);

	/*
	ScaleByAction *scale1 = new ScaleByAction(Vec2(0.6f, 0.6f), 0.2f);
	ScaleByAction *scale2 = new ScaleByAction(Vec2(-0.6f, -0.6f), 0.2f);
	ActionQueueRepeat *aq = new ActionQueueRepeat(1, 2, scale1, scale2);
	aq->infinite = true;
	exclamation->runActionQueue(aq);
	*/

	rect = actorFrames[D_DOWN];

	equip = new Sprite;
	equip->useBatchNode(GameScene::getActorSheet());
	equip->position = Vec2(14.5f, -13.f);
	equip->anchor = Vec2(0.f, 0.f);
	equip->rect = equipFrames[D_DOWN];
	addChild(equip);


	PreloadLightDef *pld = new PreloadLightDef;
	pld->radius = 320.f;
	pld->position = Vec2(-14.5f, 13.f);

	GameLayer::getSingleton()->addLight(equip, pld, "guard");
}
Guard::~Guard(void)
{
}

void Guard::prepareFrames()
{
	actorFrames[D_DOWN] = Rect(130.f, 52.f, 35.f, 52.f);
	actorFrames[D_UP] = actorFrames[D_DOWN];
	actorFrames[D_RIGHT] = actorFrames[D_DOWN];
	actorFrames[D_LEFT] = actorFrames[D_DOWN];

	equipFrames[D_DOWN] = Rect(130, 105, 14, 30);
	equipFrames[D_UP] = equipFrames[D_DOWN];
	equipFrames[D_RIGHT] = equipFrames[D_DOWN];
	equipFrames[D_LEFT] = equipFrames[D_DOWN];
}
void Guard::createBody()
{
	body = Actor::createCircularBody(
		20.f,
		CF_GUARD,
		CF_WAYPOINT|CF_WALLS|CF_PLAYER|CF_LOOT
		);
	body->SetTransform(toB2(position), 0.f);
}

void Guard::update(float dt)
{
	Actor::update(dt);

	exclamation->color.a = playerLosTimer * 2.f;
	if (playerLosTimer <= LOSTIME)
	{
		exclamation->scale = Vec2(1.f, 1.f) * (1.f + playerLosTimer);
	}

	setZOrder((int)position.y);

	if (Entity::otherColBody(body, CF_PLAYER))
	{
		GameLayer::guardCaughtPlayer();
	}

	exclamation->rect = Rect(146, 105, 13, 31);

	if (playerLos(dt) || confused)
	{
		if (!confused)
		{
			moveToPlayer();
			waiting = false;
		}
		else
		{
			exclamation->rect = Rect(166, 105, 19, 35);
			exclamation->color.a = 1.f;
			exclamation->scale = Vec2(1.5f, 1.5f);

			confuseTimer -= dt;
			vel *= 0.9f;
			if (confuseTimer <= 0.f)
			{
				confused = false;
			}
		}
	}
	else if (waiting)
	{
		if (waitLoot)
		{
			vel = Vec2(0.f, 0.f);
		}
		else
		{
			vel *= 0.9f;
		}
		waitTimer -= dt;

		if (waitTimer <= 0.f)
		{
			waiting = false;

			if (waitLoot && GameLayer::lootExists(waitLoot))
			{
				GameLayer::registerLootBag(LOOTREF_GUARD, waitLoot);
				waitLoot->pickUp();
				waitLoot = NULL;
			}
			else
			{
				waitLoot = NULL;
			}

			if (!waypointLos(curWp))
			{
				reboundToPath();
			}
		}
	}
	else if (trackingPlayer)
	{
		exclamation->rect = Rect(166, 105, 19, 35);
		exclamation->color.a = 1.f;
		exclamation->scale = Vec2(1.5f, 1.5f);

		moveToLKP();
	}
	else
	{
		LootBag *loot = lootLos(dt);
		if (loot)
		{
			moveToLoot(loot);
		}
		else
		{
			moveTo(curWp);	
		}
	}

	body->SetLinearVelocity(toB2(vel * walkSpeed));

	if (lframePos == position && !waiting)
	{
		/* In most cases this means that the guard is stuck */
		if (trackingPlayer)
		{
			trackingPlayer = false;
			reboundToPath();
		}
	}

	lframePos = position;
}

void Guard::moveTo(Waypoint *wp)
{
	walkSpeed = SPEED_PATROL;

	b2Body *wpBody = Entity::otherColBody(body, CF_WAYPOINT);
	if (wpBody && wpBody->GetUserData() == curWp)
	{
		if (!curPath.size())
		{
			curPath = patrolPath;
		}

		curWp = curPath.front();
		curPath.pop_front();
	}

	Vec2 diff = curWp->pos - position;
	vel = diff.normalize();
}
void Guard::moveToPlayer()
{
	walkSpeed = SPEED_CHASE;

	trackingPlayer = true;
	playerLKP = GameLayer::getPlayer()->position;

	Vec2 diff = playerLKP - position;
	vel = diff.normalize();
}
void Guard::moveToLoot(LootBag *loot)
{
	walkSpeed = SPEED_CHASE;

	b2Body *lootBody = Entity::otherColBody(body, CF_LOOT);
	if (lootBody && lootBody->GetUserData() == loot)
	{
		printf("Reached loot\n");
		vel = Vec2(0.f,0.f);
		waitLoot = loot;
		waiting = true;
		waitTimer = 1.f;
	}
	else
	{
		Vec2 diff = loot->position - position;
		vel = diff.normalize();
	}
}
void Guard::moveToLKP()
{
	walkSpeed = SPEED_CHASE;

	Vec2 diff = playerLKP - position;
	vel = diff.normalize();

	// If the guard is within a slight radius, end the search
	if (diff.length() < 5.f)
	{
		// If the current Waypoint isn't LOS, find a new one
		trackingPlayer = false;

		waiting = true;
		waitTimer = 1.f;
		/*
		if (!waypointLos(curWp))
		{
			printf("Old WP not LOS\n");
			reboundToPath();
		}
		*/
	}
}

bool Guard::playerLos(float dt)
{
	PlayerRCCallback cb;
	cb.ignore = body->GetFixtureList();

	b2Vec2 p1 = body->GetPosition();
	b2Vec2 p2 = GameLayer::getPlayer()->body->GetPosition();

	bool shrunk = GameLayer::getPlayer()->getShrunk();
	if (trackingPlayer && shrunk && !playerShrunkLastFrame)
	{
		confused = true; 
		confuseTimer = 1.f;
		playerShrunkLastFrame = true;
		return false;
	}

	playerShrunkLastFrame = shrunk;

	float max = 0.f;
	if (playerLosTimer > LOSTIME)
	{
		max = (shrunk)
			? (LOSLEN_SHRINK/PTMR)
			: (LOSLEN_NORM*2/PTMR);
	}
	else
	{
		max = (shrunk)
			? (LOSLEN_SHRINK/PTMR)
			: (LOSLEN_NORM/PTMR);
	}

	if (b2Distance(p1,p2) > max)
	{
		playerLosTimer = 0.f;
		return false;
	}

	GameLayer::getWorld()->RayCast(&cb, p1, p2);

	if (cb.hitPlayer())
	{
		playerLosTimer += dt;
		return (playerLosTimer > LOSTIME);
	}

	playerLosTimer = 0.f;
	return false;
}
bool Guard::waypointLos(Waypoint *wp)
{
	CategoryRCCallback cb(CF_WAYPOINT);
	
	GameLayer::getWorld()->RayCast(&cb, body->GetPosition(), toB2(wp->pos));

	return (cb.userData == wp);
}
LootBag* Guard::lootLos(float dt)
{
	vector<LootBag*> *loot = &GameLayer::getSingleton()->lootBags;
	
	LootBag *bag = NULL;
	float minDist = LOSLEN_NORM;

	for (unsigned int i=0; i<loot->size(); i++)
	{
		CategoryRCCallback cb(CF_LOOT);

		GameLayer::getWorld()->RayCast(&cb, body->GetPosition(), toB2((*loot)[i]->position));
		if (cb.userData == (*loot)[i])
		{
			float dist = abs((position - (*loot)[i]->position).length());
			if (dist < minDist)
			{
				bag = (*loot)[i];
				minDist = dist;
			}
		}
	}

	if (bag)
	{
		lootLosTimer += dt;
		if (lootLosTimer > 0.5f)
		{
			return bag;
		}
	}
	else
	{
		lootLosTimer = 0.f;
	}

	return NULL;
}

void Guard::reboundToPath()
{
	/* Find the closest WP within LOS */
	Waypoint *wp = NULL;
	float minLen = 100000.f;
	b2Vec2 pos = body->GetPosition();
	
	vector<Waypoint*> *network = &GameLayer::getSingleton()->waypoints;

	for (unsigned int i=0; i<network->size(); i++)
	{
		if (waypointLos((*network)[i]))
		{
			float curLen = abs(((*network)[i]->pos-position).length());

			if (!wp || curLen < minLen)
			{
				minLen = curLen;
				wp = (*network)[i];
			}
		}
	}

	/* Find the shortest way to the previous waypoint */
	AStar astar;
	deque<Waypoint*> tmp = astar.findPath(wp, curWp);
	for each (Waypoint *w in tmp)
	{
		curPath.push_front(w);
	}

	curWp = curPath.front();
	curPath.pop_front();
}
