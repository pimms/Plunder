#include "HighScoreLayer.h"


/* STATIC */
string HighScoreLabel::getTitle(int score)
{
	if (score == 0)			return "176-671";
	if (score < 500)		return "Kiosk robber";
	if (score < 1000)		return "Time Thief";
	if (score < 1500)		return "Kindergarten Supervillain";
	if (score < 2000)		return "Police-Bait";
	if (score < 2500)		return "Loudmouth McBad";
	if (score < 3000)		return "Apple Thief";
	if (score < 3500)		return "First-time Burglar";
	if (score < 4000)		return "Small time bandit";
	if (score < 4500)		return "Casual Cleptomaniac";
	if (score < 5000)		return "Cleptomaniac";
	if (score < 7000)		return "Quickhands McLoot";
	if (score < 7500)		return "Apiring Plunderer";
	if (score < 8000)		return "Looter";
	if (score < 10001)		return "Plunderer";	
	if (score < 10500)		return "Plunderer Boss";
	if (score < 12000)		return "Perfectionist";
	if (score < 14000)		return "MASTER PLUNDERER";

	return "Cheater bandit";
}

HighScoreLabel::HighScoreLabel(string nm, int sc, Font *ft, int scoreIdx, int posIdx)
{
	index = new Label(ft);
	name  = new Label(ft);
	score = new Label(ft);
	title = new Label(ft);

	index->setTextWithFormat("%i\n", scoreIdx+1);
	name->setText(nm);
	score->setTextWithFormat("%i\n", sc);
	title->setText(HighScoreLabel::getTitle(sc));

	index->setTextAlignment(Label::TEXT_LEFT);
	name->setTextAlignment(Label::TEXT_LEFT);
	score->setTextAlignment(Label::TEXT_LEFT);
	title->setTextAlignment(Label::TEXT_LEFT);

	index->color = Color(0.f, 0.f, 0.f, 1.f);
	name->color = Color(0.f, 0.f, 0.f, 1.f);
	score->color = Color(0.f, 0.f, 0.f, 1.f);
	title->color = Color(0.f, 0.f, 0.f, 1.f);

	index->position = Vec2(13.f, 0.f);
	name->position = Vec2(79.f, 0.f);
	score->position = Vec2(322.f, 0.f);
	title->position = Vec2(570.f, 0.f);

	addChild(index);
	addChild(name);
	addChild(score);
	addChild(title);

	// -- 

	highlight = false;
	position = Vec2(167.f, 593.f - posIdx*50.f);
}
void HighScoreLabel::draw()
{
	if (highlight)
	{
		Vec2 fac = GameControl::getSingleton()->windowScale();

		glPushMatrix();
		glLoadIdentity();

		glScalef(fac.x, fac.y, 1.f);
		glTranslatef(position.x, position.y, 0.f);
		glDisable(GL_TEXTURE_2D);

		glColor4ub(255,0,0,100);

		glBegin(GL_QUADS);
			glVertex2i(0,	-25);
			glVertex2i(943, -25);
			glVertex2i(943,  25);
			glVertex2i(0,	 25);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	GameNode::draw();
}


// ----


HighScoreLayer::HighScoreLayer()
{
	immovable = true;
	impact20 = NULL;

	scrolling = false;
	scrollIdx = 0;

	scrollViewFrom = 0;
	scrollViewTo = 9;
}
HighScoreLayer::~HighScoreLayer()
{
	delete impact20;
}

void HighScoreLayer::loadResources()
{
	impact20 = new Font("impact.ttf", 20);

	Sprite *bck = new Sprite("hs_back.png");
	bck->anchor = Vec2(0.f, 0.f);
	addChild(bck);

	loadScores();
	viewScores(0, true);

	listenKeys();
	listenController();
}

void HighScoreLayer::controllerEvent(ControllerEvent &evt)
{
	scrolling = true;

	if (evt.isKeyFresh(ControllerEvent::X_DUP))
	{
		scrollIdx--;
		if (scrollIdx < 0)
		{
			scrollIdx = 0;
		}

		viewScores(scrollIdx, true);
	}
	else if (evt.isKeyFresh(ControllerEvent::X_DDOWN))
	{
		scrollIdx++;
		if (scrollIdx > scores.size()-1)
		{
			scrollIdx = scores.size()-1;
		}

		viewScores(scrollIdx, true);
	}

	scrolling = false;
}
void HighScoreLayer::keyEvent(KeyEvent &evt)
{
	scrolling = true;

	if (evt.isKeyFresh(KeyEvent::K_UP))
	{
		scrollIdx--;
		if (scrollIdx < 0)
		{
			scrollIdx = 0;
		}

		viewScores(scrollIdx, true);
	}
	else if (evt.isKeyFresh(KeyEvent::K_DOWN))
	{
		scrollIdx++;
		if (scrollIdx > scores.size()-1)
		{
			scrollIdx = scores.size()-1;
		}

		viewScores(scrollIdx, true);
	}

	scrolling = false;
}

int HighScoreLayer::addScore(Score score)
{
	TiXmlDocument *doc = new TiXmlDocument();
	if (!doc->LoadFile("hs.pim"))
	{
		TiXmlElement *players = new TiXmlElement("entries");
		doc->LinkEndChild(players);
		doc->SaveFile("hs.pim");
	}

	TiXmlElement *entries = doc->FirstChildElement("entries");

	TiXmlElement *player = new TiXmlElement("player");
	player->SetAttribute("name", score.first.c_str());
	player->SetAttribute("score", score.second);

	entries->LinkEndChild(player);

	doc->SaveFile("hs.pim");

	delete doc;

	/*	Create a temporary vector holding the scores, sort it, and return the index of
		the newly added Score entry */
	vector<Score*> tmpScore;
	for (unsigned int i=0; i<scores.size(); i++) 
	{
		tmpScore.push_back(&scores[i]);
	}
	tmpScore.push_back(&score);

	for (unsigned int j=1; j<tmpScore.size(); j++)
	{
		Score *key = tmpScore[j];
		int i = j - 1;

		while (i >= 0 && tmpScore[i]->second < key->second)
		{
			tmpScore[i+1] = tmpScore[i];
			i--;
		}
		tmpScore[i+1] = key;
	}

	for (unsigned int i=0; i<tmpScore.size(); i++)
	{
		if (tmpScore[i] == &score)
		{
			return i;
		}
	}

	return -1;
}
void HighScoreLayer::loadScores()
{
	scores.clear();

	TiXmlDocument *doc = new TiXmlDocument();
	if (!doc->LoadFile("hs.pim"))
	{
		TiXmlElement *players = new TiXmlElement("entries");
		doc->LinkEndChild(players);
		doc->SaveFile("hs.pim");
	}
	else
	{
		TiXmlElement *entries = doc->FirstChildElement("entries");
		if (!entries)
		{
			entries = new TiXmlElement("entries");
			doc->LinkEndChild(entries);
			doc->SaveFile("hs.pim");
		}
		else
		{
			for (	TiXmlElement *player=entries->FirstChildElement("player"); 
					player;		  player=player->NextSiblingElement("player")	)
			{
				Score score;

				const char *attr = player->Attribute("name");
				if (attr)
				{
					score.first = attr;

					attr = player->Attribute("score");
					if (attr)
					{
						score.second = atoi(attr);
						scores.push_back(score);
					}
				}
			}
		}
	}

	delete doc;

	/* Sort them */
	for (unsigned int j=1; j<scores.size(); j++)
	{
		Score key = scores[j];
		int i = j - 1;

		while (i >= 0 && scores[i].second < key.second)
		{
			scores[i+1] = scores[i];
			i--;
		}
		scores[i+1] = key;
	}
}

void HighScoreLayer::viewScores(int from, bool highlight)
{
	for each (HighScoreLabel *l in labels) 
	{
		removeChild(l);
	}
	labels.clear();

	scrollIdx = from;

	if (!scrolling)
	{
		if (from < 10)
		{
			from = 0;
		}
		else if (from > scores.size()-10)
		{
			from = scores.size()-10;
		}
		else
		{
			from -= 5;
		}

		scrollViewFrom = from;
		scrollViewTo = from+9;
	}
	else
	{
		if (scrollIdx < scrollViewFrom)
		{
			scrollViewFrom = scrollIdx;
			scrollViewTo = scrollIdx+9;
		}
		else if (scrollIdx > scrollViewTo)
		{
			scrollViewTo = scrollIdx;
			scrollViewFrom = scrollIdx-9;
		}

		from = scrollViewFrom;
	}

	for (unsigned int i=from, j=0; i<scores.size() && j<10; i++, j++)
	{
		HighScoreLabel *label = new HighScoreLabel(
			scores[i].first, 
			scores[i].second, 
			impact20, i, j
		);

		if (scrollIdx == i && highlight)
		{
			label->highlight = true;	
		}

		addChild(label);
		labels.push_back(label);
	}
}