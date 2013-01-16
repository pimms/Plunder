#include "MainMenuLayer.h"
#include "HighScoreLayer.h"
#include "GameScene.h"

MainMenuLayer::MainMenuLayer(void)
{
	hsLayer = NULL;
	btnBack = NULL;
}
MainMenuLayer::~MainMenuLayer(void)
{
}

void MainMenuLayer::loadResources()
{
	listenController();

	Sprite *background = new Sprite("mainmenutex.png");
	background->anchor = Vec2(0.f, 0.f);
	background->position = Vec2(0.f, 0.f);
	addChild(background);

	btnPlay = MenuButton::create(
		"Play",
		Color(0.7f, 0.9f, 0.7f, 1.f),
		20
	);
	btnPlay->position = Vec2(480.f, 100.f);
	btnPlay->setCallback(this);
	btnPlay->addButtonGuide(kDescA, Vec2(0.f, -40.f));
	addChild(btnPlay);

	btnHighScore = MenuButton::create(
		"Highscores",
		Color(0.7f, 0.9f, 0.7f, 1.f),
		20
	);
	btnHighScore->position = Vec2(800.f, 100.f);
	btnHighScore->setCallback(this);
	btnHighScore->addButtonGuide(kDescB, Vec2(0.f, -40.f));
	addChild(btnHighScore);
}

void MainMenuLayer::buttonReleased(Button *button)
{
	if (!button)
	{
		return;
	}

	if (button == btnHighScore)
	{
		hsLayer = new HighScoreLayer;
		addChild(hsLayer);

		btnBack = MenuButton::create("Close",Color(0.7f,0.7f,0.7f,1.f),15,Vec2(0.5f,0.5f));
		btnBack->position = Vec2(640.f, 70.f);
		btnBack->setCallback(this);
		btnBack->addButtonGuide(kDescB, Vec2(0.f, -30.f));
		hsLayer->addChild(btnBack);

		btnPlay->setActive(false);
		btnHighScore->setActive(false);
	}
	else if (button == btnPlay)
	{
		GameControl::getSingleton()->setScene(new GameScene);
	}
	else if (button == btnBack)
	{
		if (hsLayer)
		{
			removeChild(hsLayer);

			hsLayer = NULL;
			btnBack = NULL; 

			btnPlay->setActive(true);
			btnHighScore->setActive(true);
		}
	}
}
void MainMenuLayer::controllerEvent(ControllerEvent &evt)
{
	if (evt.isKeyFresh(ControllerEvent::X_A))
	{
		buttonReleased(btnPlay);
	}
	else if (evt.isKeyFresh(ControllerEvent::X_B))
	{
		if (btnBack)
		{
			buttonReleased(btnBack);
		}
		else
		{
			buttonReleased(btnHighScore);
		}
	}
}