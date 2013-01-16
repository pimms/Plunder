#pragma once
#include "Pim.h"

using namespace Pim;
using namespace std;

class NameInputCallback
{
public:
	virtual void inputCompleted(string text) = 0;
};

class NameInput : public Sprite
{
public:
	NameInput(NameInputCallback *cb);
	~NameInput(void);

	void keyEvent(KeyEvent &evt);

private:
	NameInputCallback *callback;

	string	text;
	Label	*label;
	Font	*impact40;

	Sprite	*background;
};

