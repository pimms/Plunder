#pragma once
#include "Pim.h"

using namespace Pim;
using namespace std;

enum XboxDesc { kDescA, kDescB };

class MenuButton : public Button
{
public:
	static MenuButton* create(string text, Color color, int fontSize=20, Vec2 scale=Vec2(1.f,1.f));
	~MenuButton(void);

	void addButtonGuide(XboxDesc desc, Vec2 offset);

private:
	MenuButton(string text, Color color, int fSize, Sprite* normal, Sprite* hovered=NULL, Sprite* 
			pressed=NULL, Sprite* deactivated=NULL);

	void makeNormalCurrent();
	void makeHoveredCurrent();
	void makePressedCurrent();

	Font		*font;
	Label		*label;
};

