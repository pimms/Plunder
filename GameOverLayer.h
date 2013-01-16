#pragma once
#include "Pim.h"
#include "NameInput.h"
#include "HighScoreLayer.h"
#include "MenuButton.h"

using namespace Pim;

enum GameOverStatus
{
	kPlayerJailed,
	kPlayerSucceed
};

class GameOverLayer : public Layer, public NameInputCallback, public ButtonCallback
{
public:
	GameOverLayer(GameOverStatus status, int score);
	~GameOverLayer(void);

	void loadResources();

	void inputCompleted(string text);
	void buttonReleased(Button *btn);

	void controllerEvent(ControllerEvent &evt);
	void keyEvent(KeyEvent &evt);

private:
	NameInput		*input;

	HighScoreLayer	*hsLayer;

	string			title;
	Score			curScore;

	MenuButton		*btnReplay;
	MenuButton		*btnMenu;
};

