#pragma once
#include "Pim.h"

using namespace Pim;

class HUDLayer : public Layer
{
public:
	HUDLayer();
	~HUDLayer();

	void loadResources();	

	void setFraction(int num, int den);

	void displayLootSplash();
	void actionCompleted(BaseAction *action);

private:
	Font	*font;

	Label	*numerator;
	Label	*denominator;

	Sprite	*bag;
	BaseAction *lastBagAction;

	Sprite	*bagShadow;
	BaseAction *lastBagShadowAction;

	Sprite	*stamp;
	BaseAction *firstStampAction;
	BaseAction *lastStampAction;

	Sprite		*black;
	TintAction *fadeblack;
};

