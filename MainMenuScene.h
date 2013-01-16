#pragma once
#include "Pim.h"
#include "MainMenuLayer.h"

using namespace Pim;

class MainMenuScene : public Scene
{
public:
	MainMenuScene(void);
	~MainMenuScene(void);

	void loadResources();

private:
	MainMenuLayer	*menuLayer;
};

