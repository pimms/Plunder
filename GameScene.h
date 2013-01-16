#pragma once
#include "Plunder.h"
#include "GameLayer.h"
#include "GameOverLayer.h"
#include "HUDLayer.h"

using namespace Pim;

class GameScene : public Scene
{
public:
	static SpriteBatchNode* getActorSheet();
	static GameScene* getSingleton();
	static HUDLayer* getHUDLayer();
	static void gameOver(GameOverStatus status, int score);

	GameScene(void);
	~GameScene(void);

	Layer* pauseLayer();

	void loadResources();

private:
	static GameScene	*singleton;

	GameLayer			*gameLayer;
	HUDLayer			*hudLayer;

	GameOverLayer		*gameOverLayer;

	SpriteBatchNode		*actorSheet;

};

