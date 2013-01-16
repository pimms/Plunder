#include "MainMenuScene.h"


MainMenuScene::MainMenuScene(void)
{
}
MainMenuScene::~MainMenuScene(void)
{
}

void MainMenuScene::loadResources()
{
	menuLayer = new MainMenuLayer;
	addLayer(menuLayer);
}