#include "MenuButton.h"


/* Static factory */
MenuButton* MenuButton::create(string text, Color color, int fSize, Vec2 scale)
{
	Sprite *norm = new Sprite("mnbtn.png");
	norm->rect = Rect(0,0,200,140);
	norm->scale = scale;
	norm->color = color;

	Sprite *hov = new Sprite("mnbtn.png");
	hov->rect = Rect(200,0,200,140);
	hov->scale = scale;
	hov->color = color;

	Sprite *prs = new Sprite("mnbtn.png");
	prs->rect = Rect(0,140,200,140);
	prs->scale = scale;
	prs->color = color;

	Sprite *dea = new Sprite("mnbtn.png");
	dea->rect = Rect(200,140,200,140);
	dea->scale = scale;
	dea->color = color;

	return new MenuButton(text, color, fSize, norm, hov, prs, dea);
}


MenuButton::MenuButton(string text, Color color, int fSize, Sprite* normal, Sprite* hovered, 
					   Sprite* pressed, Sprite* deactivated)
	: Button(normal, hovered, pressed, deactivated)
{
	font = new Font("impact.ttf", fSize);

	label = new Label(font);
	label->color = Color(0.f, 0.f, 0.f, 1.f);
	label->setTextAlignment(Label::TEXT_CENTER);
	label->setText(text);
	addChild(label);
}
MenuButton::~MenuButton(void)
{
	delete font;
}

void MenuButton::addButtonGuide(XboxDesc desc, Vec2 offset)
{
	Sprite *guide = new Sprite("actor.png"); // fuck the system ('s resources)
	guide->position = offset;

	if (desc == kDescA)
	{
		guide->rect = Rect(197, 0, 27, 27);
	}
	else if (desc == kDescB)
	{
		guide->rect = Rect(170, 0, 27, 27);
	}

	addChild(guide);
}

void MenuButton::makeNormalCurrent()
{
	Button::makeNormalCurrent();

	label->position = Vec2(0.f, 0.f);
}
void MenuButton::makeHoveredCurrent()
{
	Button::makeHoveredCurrent();

	label->position = Vec2(0.f, 0.f);
}
void MenuButton::makePressedCurrent()
{
	Button::makePressedCurrent();

	label->position = Vec2(0.f, -7.f);
}