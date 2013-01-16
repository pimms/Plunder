#include "GuardHelperObjects.h"
#include "Player.h"
#include "Plunder.h"

/* Player RC */
PlayerRCCallback::PlayerRCCallback()
{
	fixture = NULL;
}
float32 PlayerRCCallback::ReportFixture(b2Fixture *fix, const b2Vec2 &pt, const b2Vec2 &n, float32 frac)
{
	if (fix == ignore)
	{
		return -1;
	}

	if (fix->GetFilterData().categoryBits == CF_GUARD)
	{
		return 1;
	}

	if (fix->GetFilterData().categoryBits == CF_WAYPOINT)
	{
		return 1;
	}

	fixture = fix;

	point = toPim(pt);

	return frac;
}
bool PlayerRCCallback::hitPlayer()
{
	return (fixture == GameLayer::getPlayer()->body->GetFixtureList());
}

/* Waypoint RC */
CategoryRCCallback::CategoryRCCallback(uint16 cat)
{
	category = cat;
	userData = NULL;
}
float32 CategoryRCCallback::ReportFixture(b2Fixture *fix, const b2Vec2 &pt, const b2Vec2 &norm, float32 frac)
{
	if (category != CF_WAYPOINT && fix->GetFilterData().categoryBits == CF_WAYPOINT)
	{
		return 1;
	}

	if (fix->GetFilterData().categoryBits == category)
	{
		userData = fix->GetUserData();
		return frac;
	}
	else
	{
		userData = NULL;
		return 0;
	}
}

/* A star */
AStarNode::AStarNode()
{
	g = 0.f;
	h = 0.f;

	idx = 0;
	wpt = NULL;
	parent = NULL;
}

// static declaration
vector<AStarNode> AStar::templateNodes()
{
	vector<AStarNode> nodes;

	if (!nodes.size())
	{
		printf("Creating template...\n");

		vector<Waypoint*> *network = &GameLayer::getSingleton()->waypoints;

		for (unsigned int i=0; i<network->size(); i++)
		{
			AStarNode node;

			node.pos = (*network)[i]->pos;
			node.wpt = (*network)[i];
			node.idx = i;

			nodes.push_back(node);
		}

		for (unsigned int i=0; i<network->size(); i++)
		{
			Waypoint *base = (*network)[i];
			for (unsigned int j=0; j<base->con.size(); j++)
			{
				for (unsigned int k=0; k<network->size(); k++)
				{
					if ((*network)[k] == base->con[j].wpt)
					{
						nodes[i].con.push_back(&nodes[k]);
					}
				}
			}
		}
	}

	return nodes;
}

deque<Waypoint*> AStar::findPath(Waypoint *wptA, Waypoint *wptB)
{
	vector<AStarNode> network = AStar::templateNodes();
	vector<AStarNode*> open;
	vector<AStarNode*> closed;

	AStarNode *nodeA = &network[wptA->idx];
	AStarNode *nodeB = &network[wptB->idx];
	
	for (unsigned int i=0; i<network.size(); i++)
	{
		network[i].h = abs((wptB->pos - network[i].pos).length());
	}

	open.push_back(nodeA);

	while(open.size())
	{
		AStarNode *cur = open[0];

		for each (AStarNode *n in open)		/* fuck the system */
			if (n->g < cur->g)
				cur = n;

		if (cur == nodeB)
		{
			break;
		}

		remove(open, cur);
		closed.push_back(cur);

		for each (AStarNode *n in cur->con)
		{
			if (isElement(open,n))
			{
				n->parent = cur;
				n->g = cur->g + abs((cur->pos-n->pos).length());
			}
			else if (!isElement(closed,n))
			{
				open.push_back(n);
				n->parent = cur;
				n->g = cur->g + abs((cur->pos-n->pos).length());
			}
		}
	}

	deque<Waypoint*> path;

	AStarNode *cur = nodeB;
	while (cur)
	{
		path.push_back(cur->wpt);
		cur = cur->parent;
	}

	return path;
}
