#include "GameScene.h"

using namespace Pim;

/* STATICS */
GameScene* GameScene::singleton = NULL;
SpriteBatchNode* GameScene::getActorSheet() 
{
	return singleton->actorSheet;
}
GameScene* GameScene::getSingleton()
{
	return singleton;
}
HUDLayer* GameScene::getHUDLayer()
{
	return singleton->hudLayer;
}
void GameScene::gameOver(GameOverStatus status, int score)
{
	singleton->gameOverLayer = new GameOverLayer(status, score);
	GameControl::getSingleton()->pause();
}

GameScene::GameScene(void)
{
	singleton = this;
	actorSheet = NULL;
}
GameScene::~GameScene(void)
{
	if (actorSheet)
	{
		delete actorSheet;
	}
}

Layer* GameScene::pauseLayer()
{
	return gameOverLayer;
}

void GameScene::loadResources()
{
	actorSheet = new SpriteBatchNode("actor.png");

	gameLayer = new GameLayer();
	addLayer(gameLayer);

	hudLayer = new HUDLayer();
	addLayer(hudLayer);
}
