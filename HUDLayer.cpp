#include "HUDLayer.h"
#include "GameScene.h"

HUDLayer::HUDLayer(void)
{
	font = NULL; 
}
HUDLayer::~HUDLayer(void)
{
	delete font;
}

void HUDLayer::loadResources()
{
	font = new Font("impact.ttf", 30);

	numerator = new Label(font);
	numerator->color = Color(1.f, 1.f, 0.f, 1.f);
	numerator->position = Vec2(50.f, 670.f);
	numerator->setText("0");
	addChild(numerator);

	denominator = new Label(font);
	denominator->color = Color(1.f, 1.f, 0.f, 1.f);
	denominator->position = Vec2(90.f, 630.f);
	denominator->setText("10");
	addChild(denominator);

	Sprite *line = new Sprite;
	line->rect = Rect(0,0,50,8);
	line->color = Color(1.f, 1.f, 0.f, 1.f);
	line->rotation = 30.f;
	line->position = Vec2(70.f, 650.f);
	addChild(line);

	/* Load the screen from black */
	black = new Sprite;
	black->rect = Rect(0, 0, 1280, 720);
	black->color = Color(0.f, 0.f, 0.f, 1.f);
	black->anchor = Vec2(0.f, 0.f);
	addChild(black);

	fadeblack = new TintAction(Color(0.f, 0.f, 0.f, 0.f), 1.f);
	black->runAction(fadeblack);
}

void HUDLayer::setFraction(int num, int den)
{
	if (num >= 0)
	{
		numerator->setTextWithFormat("%i\n", num);
	}

	if (den >= 0)
	{
		denominator->setTextWithFormat("%i\n", den);
	}
}

void HUDLayer::displayLootSplash()
{
	/* BAG */
	bag = new Sprite();
	bag->useBatchNode(GameScene::getActorSheet());
	bag->scale = Vec2(0.25f, 0.25f);
	bag->color = Color(1.f, 1.f, 1.f, 0.f);
	bag->position = Vec2(640.f, 360.f);
	bag->rect = Rect(294, 0, 219, 254);
	addChild(bag);

	RotateByAction *rotate = new RotateByAction(1080.f, 1.f);
	bag->runAction(rotate);

	TintAction *tint = new TintAction(Color(1.f, 1.f, 1.f, 1.f), 0.2f);
	bag->runAction(tint);

	ScaleToAction *scale = new ScaleToAction(Vec2(1.f, 1.f), 1.f);
	DelayAction *delay = new DelayAction(1.f);
	lastBagAction = new TintAction(Color(1.f, 1.f, 1.f, 0.f), 0.5f);
	ActionQueue *aq = new ActionQueue(3, scale, delay, lastBagAction);
	bag->runActionQueue(aq);

	lastBagAction->notifyOnCompletion = true;
	lastBagAction->notificationCallback = this;

	/* BAG SHADOW */
	bagShadow = new Sprite();
	bagShadow->useBatchNode(GameScene::getActorSheet());
	bagShadow->color = Color(1.f, 1.f, 1.f, 0.f);
	bagShadow->position = Vec2(640.f, 200.f);
	bagShadow->rect = Rect(8, 253, 279, 108);
	addChild(bagShadow);

	DelayAction *delay2 = new DelayAction(0.8f);
	TintAction *fadein = new TintAction(Color(1.f, 1.f, 1.f, 1.f), 0.2f);
	DelayAction *delay3 = new DelayAction(1.f);
	lastBagShadowAction = new TintAction(Color(1.f, 1.f, 1.f, 0.f), 0.5f);
	ActionQueue *aq2 = new ActionQueue(4, delay2, fadein, delay3, lastBagShadowAction);
	bagShadow->runActionQueue(aq2);

	lastBagShadowAction->notifyOnCompletion = true;
	lastBagShadowAction->notificationCallback = this;

	/* STAMP */
	stamp = new Sprite();
	stamp->useBatchNode(GameScene::getActorSheet());
	stamp->color = Color(1.f, 1.f, 1.f, 0.f);
	stamp->position = Vec2(640.f, 360.f);
	stamp->rect = Rect(304, 272, 411, 221);
	stamp->scale = Vec2(3.f, 3.f);
	stamp->rotation = 15.f;
	addChild(stamp);

	firstStampAction = new DelayAction(0.8f);
	ScaleToAction *scale2 = new ScaleToAction(Vec2(1.f, 1.f), 0.4f);
	DelayAction *delay4 = new DelayAction(0.8f);
	lastStampAction = new TintAction(Color(1.f, 1.f, 1.f, 0.f), 0.5f);
	ActionQueue *aq3 = new ActionQueue(4, firstStampAction, scale2,  delay4, lastStampAction);
	stamp->runActionQueue(aq3);

	firstStampAction->notificationCallback = this;
	firstStampAction->notifyOnCompletion = true;
	lastStampAction->notificationCallback = this;
	lastStampAction->notifyOnCompletion = true;
}
void HUDLayer::actionCompleted(BaseAction *action)
{
	if (action == lastBagAction)
	{
		removeChild(bag);
		lastBagAction = NULL;
		bag = NULL;
	}
	else if (action == lastBagShadowAction)
	{
		removeChild(bagShadow);
		lastBagShadowAction = NULL;
		bagShadow = NULL;
	}
	else if (action == firstStampAction)
	{
		firstStampAction = NULL;

		TintAction *tint = new TintAction(Color(1.f, 1.f, 1.f, 1.f), 0.4f);
		stamp->runAction(tint);
	}
	else if (action == lastStampAction)
	{
		removeChild(stamp);
		lastStampAction = NULL;
		stamp = NULL;
	}
	else if (action == fadeblack)
	{
		removeChild(black);
	}
}