#pragma once
#include "Pim.h"
#include "MenuButton.h"

using namespace Pim;

class HighScoreLayer;

class MainMenuLayer : public Layer, public ButtonCallback
{
public:
	MainMenuLayer(void);
	~MainMenuLayer(void);

	void loadResources();

	void buttonReleased(Button *button);
	void controllerEvent(ControllerEvent &evt);

private:
	MenuButton		*btnPlay;
	MenuButton		*btnHighScore;

	HighScoreLayer	*hsLayer;
	MenuButton		*btnBack;
};

