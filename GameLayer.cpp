#include "GameLayer.h"
#include "GameScene.h"
#include "Player.h"
#include "Chest.h"
#include "LootBag.h"
#include "Lever.h"
#include "Door.h"
#include "Underground.h"
#include "Guard.h"
#include "BanditChest.h"
#include "MainMenuScene.h"
#include "Settings.h"

using namespace Pim;

#define MIN_CAM_X (400.f) 
#define MAX_CAM_X (880.f)
#define MIN_CAM_Y (320.f)
#define MAX_CAM_Y (400.f)

/* DEBUG DRAW FLAGS */
//#define DBGDRW_PHYS
//#define DBGDRW_WP

/* STATICS */
GameLayer* GameLayer::singleton = NULL;

b2World* GameLayer::getWorld()
{
	if (singleton)
	{
		return singleton->world;
	}

	return NULL;
}
GameLayer* GameLayer::getSingleton()
{
	return singleton;
}
Player* GameLayer::getPlayer()
{
	if (singleton)
	{
		return singleton->player;
	}

	return NULL;
}
void GameLayer::lootAddedToChest()
{
	singleton->capturedLoot++;
	lootDeliveryCheck();

	GameScene::getHUDLayer()->displayLootSplash();
}
void GameLayer::registerLootBag(int action, LootBag *loot)
{
	if (action == LOOTREF_REMOVE)
	{
		for (unsigned int i=0; i<singleton->lootBags.size(); i++)
		{
			if (singleton->lootBags[i] == loot)
			{
				singleton->lootBags.erase(singleton->lootBags.begin() + i);	
			}
		}
	}
	else if (action == LOOTREF_ADD)
	{
		singleton->lootBags.push_back(loot);
	}
	else if (action == LOOTREF_GUARD)
	{
		for (unsigned int i=0; i<singleton->lootBags.size(); i++)
		{
			if (singleton->lootBags[i] == loot)
			{
				singleton->lootBags.erase(singleton->lootBags.begin() + i);	
			}
		}

		singleton->availableLoot--;
	}

	lootDeliveryCheck();
}
void GameLayer::lootDeliveryCheck()
{
	printf("Loot in bandit house: %i/%i\n", singleton->capturedLoot, singleton->availableLoot);

	GameScene::getHUDLayer()->setFraction(singleton->capturedLoot, singleton->availableLoot);

	if (singleton->capturedLoot == singleton->availableLoot)
	{
		playerLootedAll();
	}
}
bool GameLayer::lootExists(LootBag *loot)
{
	for (unsigned int i=0; i<singleton->lootBags.size(); i++)
	{
		if (singleton->lootBags[i] == loot)
		{
			return true;
		}
	}

	return false;
}
void GameLayer::scaleTo(float dst)
{
	singleton->scaleDest = dst;
	singleton->scaling = true;
}
int GameLayer::score()
{
	int sec = (int)singleton->timer;
	sec = 600-sec;
	if (sec < 0) sec = 0;

	int missing = 10-singleton->availableLoot;
	int captured = singleton->capturedLoot;

	int score = (pow(captured,2)*100) - (missing*50) + (sec*captured/2.f);

	if (score < 0 || singleton->capturedLoot == 0)
	{
		score = 0;
	}

	return score;
}
void GameLayer::guardCaughtPlayer()
{
	GameScene::getSingleton()->gameOver(kPlayerJailed, score());
}
void GameLayer::playerLootedAll()
{
	GameScene::getSingleton()->gameOver(kPlayerSucceed, score());
}

GameLayer::GameLayer(void)
{
	singleton = this;
	world = NULL;
	scaling = false;
	scaleDest = 1.f;

	timer = 0.f;
	capturedLoot = 0;
	availableLoot = 10;
}
GameLayer::~GameLayer(void)
{
	singleton = NULL;

	if (world)
	{
		delete world;
		world = NULL;
	}

	for (unsigned int i=0; i<waypoints.size(); i++)
	{
		delete waypoints[i];
	}
}

void GameLayer::loadResources()
{
	listenFrame();
	listenInput();
	//listenController();

	world = new b2World(b2Vec2_zero);

	loadLightingSystem();
	loadLevelData();
	loadSprites();
	loadChests();
	loadDoorsAndLevers();
	loadUndergrounds();

	player = new Player;
	addChild(player);

	/* Add the guards */
	for (int i=0; i<10; i++)
	{
		addChild(new Guard(i));
	}
}
void GameLayer::loadLightingSystem()
{
	createLightingSystem(Vec2(1280.f, 720.f));
	setSmoothShadows(globalSettings.smoothShadows);
	setLightingUnlitColor(Color(0.f, 0.f, 0.f, 0.85f));
	setLightAlpha(0.1f);
	setCastShadows(globalSettings.castShadows);

	LightDef *ld = new SmoothLightDef;
	ld->radius = 450.f;
	ld->falloff = 0.2f;
	ld->castShadows = true;
	ld->innerColor = Color(1.f, 0.8f, 0.8f, 1.f);
	ld->outerColor = Color(1.f, 0.f, 0.f, 0.f);
	preloadLightTexture(ld, "default");

	ld = new FlatLightDef;
	ld->radius = 250.f;
	ld->falloff = 0.6f;
	ld->castShadows = true;
	ld->innerColor = Color(1.f, 0.8f, 0.8f, 1.f);
	ld->outerColor = Color(1.f, 0.f, 0.f, 0.f);
	preloadLightTexture(ld, "guard");


	Vec2 lightPos[14] = {
		Vec2(3071.f, 2230.f),
		Vec2(3571.f, 2267.f),
		Vec2(3416.f, 1283.f),
		Vec2(1860.f, 2253.f),
		Vec2(2182.f, 3148.f),
		Vec2(903.7f, 1137.f),
		Vec2(467.7f, 1829.f),
		Vec2(470.9f, 2145.f),
		Vec2(594.f, 3038.f),
		Vec2(3215.f, 3019.f),
		Vec2(3121.f, 31019.f),
		Vec2(3008.f, 3014.f),
		Vec2(3339.f, 3020.f),
		Vec2(4272.f, 3037.f),
	};

	for (int i=0; i<14; i++)
	{
		PreloadLightDef *pld = new PreloadLightDef;
		pld->radius = 450.f;
		pld->innerColor = Color(1.f, 0.f, 0.f, 1.f);

		GameNode *light = new GameNode;	
		light->position = lightPos[i];
		addChild(light);
		addLight(light, pld, "default");
	}
}
void GameLayer::loadLevelData()
{
	Layer *tmpLayer = new Layer;

	LevelParser parser;
	if (!parser.parse("pllvl.pim", tmpLayer))
	{
		delete tmpLayer;
		throw "Could not load 'pllvl.pim'!";
		return;
	}

	delete tmpLayer;
	LevelData data = parser.data;
	
	createStaticBodies(data.physics);
	createShadowObjects(data.shadows);

	// ---

	PathNodeManager mgr;
	waypoints = mgr.read("path.pim");

	for (unsigned int i=0; i<waypoints.size(); i++)
	{
		b2CircleShape shape;
		shape.m_radius = 16.f / PTMR;

		b2FixtureDef fd;
		fd.shape				= &shape;
		fd.isSensor				= true;
		fd.filter.categoryBits	= CF_WAYPOINT;
		fd.filter.maskBits		= ~0;
		fd.userData				= waypoints[i];

		b2BodyDef bd;
		bd.position = toB2(waypoints[i]->pos);
		bd.userData = waypoints[i];

		world->CreateBody(&bd)->CreateFixture(&fd);
	}
}
void GameLayer::loadSprites()
{
	// Load the ground texture
	groundSprite = new Sprite("pltex.png");
	groundSprite->anchor = Vec2(0.f, 0.f);
	groundSprite->setZOrder(10000);
	addChild(groundSprite);
}
void GameLayer::loadChests()
{
	Vec2 chestPos[10] = {
		Vec2(4654.f, 2223.f),	// House for sale
		Vec2(496.f, 3035.f),	// Super best groceries
		Vec2(1903.f, 3222.f),	// Honest Nick's
		Vec2(4307.f, 3197.f),	// GT & FO
		Vec2(573.f, 2240.f),	// Pretty as F*!
		Vec2(1044.f, 1108.f),	// Æsj Mat
		Vec2(3013.f, 3180.f),	// Superbank 2k
		Vec2(3263.f, 3180.f),	// SUperbank 2k
		Vec2(3231.f, 833.f),	// Police Station
		Vec2(2442.f, 2248.f),	// Public library
	};

	for (int i=0; i<10; i++)
	{
		Chest *chest = new Chest(chestPos[i]);
		addChild(chest);
	}

	BanditChest *bdChest = new BanditChest(Vec2(3447.f, 2176.f));
	addChild(bdChest);
}
void GameLayer::loadDoorsAndLevers()
{
	Lever *lever;
	Door *door;

	/* Bandit Lair */
	lever = new Lever(Vec2(3076.f, 1685.f));
	door = new Door(Vec2(3308.f, 1626.f), D_RIGHT, true);
	lever->door = door;
	addChild(lever); addChild(door);

	/* House for sale */
	lever = new Lever(Vec2(4124.f, 1717.f));
	door = new Door(Vec2(4022.f, 1983.f), D_DOWN, true);
	lever->door = door;
	addChild(lever); addChild(door);

	/* Super beat groceries */
	lever = new Lever(Vec2(496.f, 2792));
	door = new Door(Vec2(1144.f, 2969.f), D_UP, true);
	lever->door = door;
	addChild(lever); addChild(door);

	/* Superbank 2k */
	lever = new Lever(Vec2(3493.f, 2666.f));
	door = new Door(Vec2(3320.f, 2598.f), D_RIGHT, true);
	lever->door = door;
	addChild(lever); addChild(door);

	/* Chinese restaurant */
	lever = new Lever(Vec2(550.f, 667.f));
	door = new Door(Vec2(742.f, 602.f), D_RIGHT, true);
	lever->door = door;
	addChild(lever); addChild(door);

	/* Honest Nick's */
	lever = new Lever(Vec2(1999.f, 3222.f));
	door = new Door(Vec2(2159.f, 2600.f), D_RIGHT, true);
	lever->door = door;
	addChild(lever); addChild(door);

	/* Get There & Fly Out */
	lever = new Lever(Vec2(4116.f, 2662.f));
	door = new Door(Vec2(4399.f, 2600.f), D_RIGHT, true);
	lever->door = door;
	addChild(lever); addChild(door);

	/* Pretty as Fuck */
	lever = new Lever(Vec2(1029.f, 2083.f));
	door = new Door(Vec2(1139.f, 1964.f), D_DOWN, true);
	lever->door = door;
	addChild(lever); addChild(door);
}
void GameLayer::loadUndergrounds()
{
	Underground *ug;

	/* Police Station <-> Superbank 2k */
	ug = new Underground(Vec2(3604.f,1278.f), Vec2(3611.f, 2616.f));
	addChild(ug);

	/* Public Library <-> Honest Nick's */
	ug = new Underground(Vec2(1815.f,2263.f), Vec2(1833.f,2621.f));
	addChild(ug);

	/* Honest Nick's <-> Get There & Fly Out */
	ug = new Underground(Vec2(2503.f,3238.f), Vec2(4022.f,3237.f));
	addChild(ug);

	/* Chinese Restaurant <-> Pretty as Fuck */
	ug = new Underground(Vec2(1117.f, 1253.f), Vec2(1120.f, 1640.f));
	addChild(ug);
}

void GameLayer::createStaticBodies(Polygons &poly)
{
	for (int i=0; i<poly.size(); i++)
	{
		b2Vec2 verts[3] = {
			toB2(poly[i][0]/3.333f),
			toB2(poly[i][1]/3.333f),
			toB2(poly[i][2]/3.333f),
		};

		// Ensure that all polygons are wound CCW
		for (int i=0; i<3; i++)
		{
			int i2 = (i+1 < 3) ? (i+1) : (0);
			b2Vec2 edge = verts[i2] - verts[i];

			for (int j=0; j<3; j++)
			{
				if (j == i || j == i2)
				{
					continue;
				}

				b2Vec2 r = verts[j] - verts[i];
				if (b2Cross(edge, r) <= 0.f)
				{
					b2Vec2 tmp = verts[j];
					verts[j] = verts[i];
					verts[i] = tmp;
				}
			}
		}

		b2BodyDef bd;
		bd.type = b2_staticBody;

		b2PolygonShape shape;
		shape.Set(verts, 3);

		b2FixtureDef fd;
		fd.shape				= &shape;
		fd.filter.categoryBits	= CF_WALLS;
		fd.filter.maskBits		= ~0;

		b2Body *body = world->CreateBody(&bd);
		body->CreateFixture(&fd);
	}
}
void GameLayer::createShadowObjects(Polygons &poly)
{
	for (int i=0; i<poly.size(); i++)
	{
		Vec2 verts[3] = {
			poly[i][0] / 3.333f,
			poly[i][1] / 3.333f,
			poly[i][2] / 3.333f
		};

		GameNode *obj = new GameNode;
		addChild(obj);

		obj->setShadowShape(verts, 3);
		
		addShadowCaster(obj);
	}
}

void GameLayer::draw()
{
	Layer::draw();

#ifdef DBGDRW_PHYS
	glPushMatrix();
	glLoadIdentity();

	Pim::Vec2 fac = Pim::GameControl::getSingleton()->coordinateFactor();
	glTranslatef(position.x / fac.x, position.y / fac.y, 0.f);

	fac = Pim::GameControl::getSingleton()->windowScale();
	glScalef(scale.x * fac.x * PTMR, scale.y * fac.y * PTMR, 1.f);

	glColor4ub(255,0,0,80);

	glDisable(GL_TEXTURE_2D);

	for (auto b=world->GetBodyList(); b; b=b->GetNext())
	{
		for (auto f=b->GetFixtureList(); f; f=f->GetNext())
		{
			// Is the body a polygon shape?
			b2PolygonShape *poly = dynamic_cast<b2PolygonShape*>(f->GetShape());
			if (poly)
			{
				glPushMatrix();
				glTranslatef(b->GetPosition().x, b->GetPosition().y, 0.f);
				glRotatef(b->GetAngle()*RADTODEG, 0.f, 0.f, 1.f);

				glBegin(GL_POLYGON);
					for (int i=0; i<poly->GetVertexCount(); i++)
					{
						glVertex2f(poly->GetVertex(i).x, poly->GetVertex(i).y);
					}
				glEnd();

				glPopMatrix();
			}

			// Is the body a circular shape?
			b2CircleShape *circle = dynamic_cast<b2CircleShape*>(f->GetShape());
			if (circle)
			{
				glPushMatrix();
				glTranslatef(b->GetPosition().x, b->GetPosition().y, 0.f);
				
				float rad = circle->m_radius;

				glBegin(GL_TRIANGLE_FAN);
					glVertex2f(0.f, 0.f);

					for (float a=0.f; a<2*M_PI; a+=(2*M_PI)/32.f)
					{
						glVertex2f( rad*cosf(a), rad*sinf(a) );
					}

					glVertex2f( rad*cosf(0.f), rad*sinf(0.f) );
				glEnd();

				glPopMatrix();
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
#endif /* !DBGDRW_PHYS! */

#ifdef DBGDRW_WP
	/* DRAW THE WAYPOINTS */
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.f);
	glScalef(scale.x, scale.y, 1.f);

	glPointSize(30.f);
	glLineWidth(5.f);

	/* Draw the connective lines */
	glColor4ub(0,0,0,150);
	glBegin(GL_LINES);
		for (unsigned int i=0; i<waypoints.size(); i++)
		{
			Waypoint *way = waypoints[i];
			for (unsigned int k=0; k<way->con.size(); k++)
			{
				Waypoint *rel = way->con[k].wpt;

				glVertex2f(way->pos.x, way->pos.y);
				glVertex2f(rel->pos.x, rel->pos.y);
			}
		}
	glEnd();

	/* Draw the nodes */
	glColor4ub(100,255,100,150);
	glBegin(GL_POINTS);
		for (unsigned int i=0; i<waypoints.size(); i++)
		{
			glVertex2f(waypoints[i]->pos.x, waypoints[i]->pos.y);
		}
	glEnd();

	glPopMatrix();
#endif /* !DBGDRW_WP! */
}
void GameLayer::update(float dt)
{
	if (willDelete)
	{
		return;
	}

	timer += dt;

	// Update the physics
	world->Step(dt, 10, 10);

	// Iterate over all the bodies, update position and rotation
	for (auto it=world->GetBodyList(); it; it=it->GetNext())
	{
		void *ud = it->GetUserData();
		if (ud)
		{
			Entity *ent = static_cast<Entity*>(ud);

			if (ent->body == it && ent->body->GetType() == b2_dynamicBody)
			{
				ent->position = toPim(ent->body->GetPosition()) + ent->bodyOffset;
			}
		}
	}

	// Update the scale (manual ScaleToAction)
	if (scaling)
	{
		if (scale.x < scaleDest)
		{
			scale += Vec2(dt, dt);
			position -= player->position * dt;

			if (scale.x >= scaleDest)
			{
				position += player->position * dt;
				scale = Vec2(scaleDest, scaleDest);
				scaling = false;
			}
		}
		else if (scale.x > scaleDest)
		{
			scale -= Vec2(dt, dt);
			position += player->position * dt;

			if (scale.x <= scaleDest)
			{
				position -= player->position * dt;
				scale = Vec2(scaleDest, scaleDest);
				scaling = false;
			}
		}
		else
		{
			scale = Vec2(scaleDest, scaleDest);
			scaling = false;
		}
	}

	// Move the layer to follow the player
	Vec2 offset = (player->position*scale + position) ;

	if (offset.x < MIN_CAM_X) 
	{
		position.x = (-player->position.x*scale.x + MIN_CAM_X);
	}
	else if (offset.x > MAX_CAM_X)
	{
		position.x = (-player->position.x*scale.x + MAX_CAM_X);
	}

	if (offset.y < MIN_CAM_Y) 
	{
		position.y = (-player->position.y*scale.y + MIN_CAM_Y);
	}
	else if (offset.y > MAX_CAM_Y)
	{
		position.y = (-player->position.y*scale.y + MAX_CAM_Y);
	}
}

void GameLayer::mouseEvent(MouseEvent &evt)
{
	if (evt.isKeyFresh(MouseEvent::MBTN_RIGHT))
	{
		Vec2 p = evt.getPosition() - position;
		printf("MPOS: %0.1f %0.1f\n", p.x, p.y);
	}
	if (evt.isKeyFresh(MouseEvent::MBTN_LEFT))
	{
		GameNode *light = new GameNode;
		light->position = evt.getPosition() - position;

		PreloadLightDef *ld = new PreloadLightDef;
		ld->radius = 450.f;

		addChild(light);
		addLight(light, ld, "default");
	}
}
void GameLayer::keyEvent(KeyEvent &evt)
{
	if (evt.isKeyFresh(KeyEvent::K_F5) || evt.isKeyFresh(KeyEvent::K_ESC))
	{
		GameControl::getSingleton()->setScene(new MainMenuScene);
	}


	/*
	if (evt.isKeyDown(KeyEvent::K_W))
	{
		position.y -= 5.f;
	}
	if (evt.isKeyDown(KeyEvent::K_S))
	{
		position.y += 5.f;
	}
	if (evt.isKeyDown(KeyEvent::K_A))
	{
		position.x += 5.f;
	}
	if (evt.isKeyDown(KeyEvent::K_D))
	{
		position.x -= 5.f;
	}
	*/
}