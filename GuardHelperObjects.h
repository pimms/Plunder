#pragma once
#include "GameLayer.h"

class PlayerRCCallback : public b2RayCastCallback
{
public:
	PlayerRCCallback();
	float32 ReportFixture(b2Fixture *fix, const b2Vec2 &pt, const b2Vec2 &norm, float32 frac);
	bool hitPlayer();

	Vec2		point;
	b2Fixture	*ignore;
	b2Fixture	*fixture;
};

class CategoryRCCallback : public b2RayCastCallback
{
public:
	CategoryRCCallback(uint16 cat);
	float32 ReportFixture(b2Fixture *fix, const b2Vec2 &pt, const b2Vec2 &norm, float32 frac);

	void *userData;
	uint16 category;
};


struct AStarNode
{
	AStarNode();
	float f() { return g+h; }

	Vec2 pos;
	float g;
	float h;

	int idx;
	Waypoint *wpt;

	AStarNode *parent;
	vector<AStarNode*> con;
};

class AStar 
{
public:
	static vector<AStarNode> templateNodes();

	deque<Waypoint*> findPath(Waypoint *wptA, Waypoint *wptB);
};


// Vector functions adding and removing elements
template<class T>
inline void remove(std::vector<T> &vec, T e)
{
	for (unsigned int i=0; i<vec.size(); i++)
	{
		if (vec[i] == e)
		{
			vec.erase(vec.begin()+i);
			return;
		}
	}
}
template<class T>
inline bool isElement(std::vector<T> &vec, T e)
{
	for each (T t in vec)
		if (t == e)
			return true;
	return false;
}
