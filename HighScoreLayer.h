#pragma once
#include "Pim.h"
#include "Plunder.h"

using namespace Pim;
using namespace std;

typedef pair<string,int> Score;

class HighScoreLabel : public GameNode
{
public:
	static string getTitle(int score);

	HighScoreLabel(string nm, int sc, Font *ft, int scoreIndex, int posIndex);
	void draw();

	bool	highlight;

private:
	Label	*index;
	Label	*name;
	Label	*score;
	Label	*title;
};

class HighScoreLayer : public Layer
{
public:
	HighScoreLayer();
	~HighScoreLayer();

	void loadResources();

	void controllerEvent(ControllerEvent &evt);
	void keyEvent(KeyEvent &evt);

	int addScore(Score score);
	void loadScores();

	void viewScores(int from, bool highlight=false);

private:
	vector<Score>				scores;

	vector<HighScoreLabel*>		labels;

	Font						*impact20;

	bool						scrolling;
	int							scrollIdx;
	int							scrollViewFrom;
	int							scrollViewTo;
};
