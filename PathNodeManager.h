#pragma once

#include <string>
#include "Pim.h"

using namespace std;
using namespace Pim;

// Representats a node
struct Waypoint
{
	struct Connection
	{
		Waypoint *wpt;
		float dst;
	};

	Vec2 pos;
	vector<Connection> con;
	int idx;
};

class PathNodeManager
{
public:
	vector<Waypoint*> read(const char*);
	Vec2 vecFromStr(const char *);

	bool write(vector<Waypoint*>,const char*);
};

