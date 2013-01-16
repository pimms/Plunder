#pragma once
#include "Plunder.h"
#include "PathNodeManager.h"

using namespace Pim;

class LootBag;
class Player;

#define LOOTREF_REMOVE	0
#define LOOTREF_ADD		1
#define LOOTREF_GUARD	2

class GameLayer : public Layer
{
public:
	static b2World* getWorld();
	static GameLayer* getSingleton();
	static Player* getPlayer();
	static void lootAddedToChest();
	static void registerLootBag(int action, LootBag *loot);
	static void lootDeliveryCheck();
	static bool lootExists(LootBag *loot);
	static void scaleTo(float dst);

	static int score();
	static void guardCaughtPlayer();
	static void playerLootedAll();
	
	GameLayer(void);
	~GameLayer(void);

	void loadResources();
	void loadLightingSystem();
	void loadLevelData();
	void loadSprites();
	void loadChests();
	void loadDoorsAndLevers();
	void loadUndergrounds();

	void createStaticBodies(Polygons &poly);
	void createShadowObjects(Polygons &poly);

	void draw();
	void update(float dt);
	
	void mouseEvent(MouseEvent &evt);
	void keyEvent(KeyEvent &evt);

	vector<Waypoint*>		waypoints;
	std::vector<LootBag*>	lootBags;

private:
	static GameLayer*	singleton;

	float				timer;
	int					capturedLoot;
	int					availableLoot;

	bool				scaling;
	float				scaleDest;

	Sprite				*groundSprite;
	Player				*player;

	b2World				*world;
};

