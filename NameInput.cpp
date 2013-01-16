#include "NameInput.h"


NameInput::NameInput(NameInputCallback *cb)
{
	callback = cb;

	impact40 = new Font("impact.ttf", 40);

	background = new Sprite("hs_name.png");
	background->anchor = Vec2(0.f, 0.f);
	addChild(background);

	label = new Label(impact40);
	label->setTextAlignment(Label::TEXT_CENTER);
	label->position = Vec2(640.f, 286.f);
	label->color = Color(0.f, 0.f, 0.f, 1.f);
	addChild(label);

	listenKeys();
}
NameInput::~NameInput(void)
{
	delete impact40;
}

void NameInput::keyEvent(KeyEvent &evt)
{
	for (int i=KeyEvent::K_A; i<KeyEvent::K_Z; i++)
	{
		if (evt.isKeyFresh((KeyEvent::KeyCode)i))
		{
			text += toupper((char)i);
		}
	}

	if (evt.isKeyDown(KeyEvent::K_BACKSPACE))
	{
		if (text.length() > 0)
		{
			text = text.substr(0, text.length()-1);
		}
	}

	if (evt.isKeyFresh(KeyEvent::K_SPACE))
	{
		text += " ";
	}

	label->setText(text);

	if (evt.isKeyFresh(KeyEvent::K_ENTER))
	{
		callback->inputCompleted(text);
	}
}