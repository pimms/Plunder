#include "GameOverLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"

GameOverLayer::GameOverLayer(GameOverStatus status, int score)
{
	immovable = true;

	curScore.second = score;
}
GameOverLayer::~GameOverLayer(void)
{
}

void GameOverLayer::loadResources()
{
	listenKeys();
	listenController();

	hsLayer = new HighScoreLayer();
	addChild(hsLayer);

	input = new NameInput(this);
	addChild(input);
	
	btnReplay = MenuButton::create(
		"Play\nagain", 
		Color(0.7f, 0.7f, 0.7f, 1.f), 
		15,
		Vec2(0.5f, 0.5f)
	);
	btnReplay->position = Vec2(1016.f, 70.f);
	btnReplay->setCallback(this);
	btnReplay->setActive(false);
	btnReplay->addButtonGuide(kDescA, Vec2(0.f, -25.f));
	addChild(btnReplay);

	btnMenu = MenuButton::create(
		"Main\nmenu",
		Color(0.7f, 0.7f, 0.7f, 1.f),
		15,
		Vec2(0.5f, 0.5f)
	);
	btnMenu->position = Vec2(262.f, 70.f);
	btnMenu->setCallback(this);
	btnMenu->setActive(false);
	btnMenu->addButtonGuide(kDescB, Vec2(0.f, -25.f));
	addChild(btnMenu);
}

void GameOverLayer::inputCompleted(string text)
{
	if (text.length() < 3)
	{
		printf("NAME TOO SHORT\n");
		return;
	}
	else if (text.length() > 16)
	{
		printf("NAME TOO LONG\n");
		return;
	}

	btnReplay->setActive(true);
	btnMenu->setActive(true);

	removeChild(input);
	input = NULL;

	curScore.first = text;

	int idx = hsLayer->addScore(curScore);
	hsLayer->loadScores();
	hsLayer->viewScores(idx, idx);
}
void GameOverLayer::buttonReleased(Button *btn)
{
	if (btn == btnReplay)
	{
		GameControl::getSingleton()->unpause();
		GameControl::getSingleton()->setScene(new GameScene);
	}
	else if (btn == btnMenu)
	{
		GameControl::getSingleton()->unpause();
		GameControl::getSingleton()->setScene(new MainMenuScene);
	}
}

void GameOverLayer::controllerEvent(ControllerEvent &evt)
{
	if (input)
	{
		if (evt.isKeyFresh(ControllerEvent::X_B))
		{
			removeChild(input);
			input = NULL;

			btnReplay->setActive(true);
			btnMenu->setActive(true);
		}
	}
	else
	{
		if (evt.isKeyFresh(ControllerEvent::X_A))
		{
			buttonReleased(btnReplay);
		}
		else if (evt.isKeyFresh(ControllerEvent::X_B))
		{
			buttonReleased(btnMenu);
		}
	}
}
void GameOverLayer::keyEvent(KeyEvent &evt)
{
	if (input && evt.isKeyDown(KeyEvent::K_ESC))
	{
		removeChild(input);
		input = NULL;

		btnReplay->setActive(true);
		btnMenu->setActive(true);
	}
}