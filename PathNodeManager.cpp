#include "PathNodeManager.h"
#include <sstream>

vector<Waypoint*> PathNodeManager::read(const char *file)
{
	vector<Waypoint*> network;
	TiXmlDocument *doc = new TiXmlDocument(file);

	if (!doc->LoadFile())
	{
		printf("COULD NOT LOAD %s!!!\n", file);
		return network;
	}

	/* Parse the nodes and their positions */
	TiXmlElement *elem = doc->FirstChildElement("node");
	int i=0;
	while (elem)
	{
		Waypoint *node = new Waypoint;
		node->idx = i++;
		network.push_back(node);

		const char *attr = elem->Attribute("pos", (double*)NULL);
		if (attr)
		{
			node->pos = vecFromStr(attr);
		}

		elem = elem->NextSiblingElement("node");
	}
	
	/* Parse relations */
	i=0;
	elem = doc->FirstChildElement("node");
	while (elem)
	{
		TiXmlElement *rel = elem->FirstChildElement("rel");
		while (rel)
		{
			const char *attr = rel->Attribute("idx");
			if (attr)
			{
				int idx = atoi(attr);

				if (idx < 0 || idx < network.size())
				{
					attr = rel->Attribute("dst");

					Waypoint::Connection c;
					c.wpt = network[idx];
					c.dst = (network[i]->pos-c.wpt->pos).length();

					network[i]->con.push_back(c);
				}
				else
				{
					printf("\n\tCORRUPTION DETECTED! (node #%i)\n\n", i);
				}
			}

			rel = rel->NextSiblingElement("rel");
		}

		elem = elem->NextSiblingElement("node");
		i++;
	}

	delete doc;

	return network;
}
Vec2 PathNodeManager::vecFromStr(const char *str)
{
	const char *x = str;
	const char *y = str;

	for (int i=0; i<strlen(str); i++)
	{
		if (str[i] == ' ')
			y = str+i;
	}

	return Vec2( 
		(float)atof(x), 
		(float)atof(y) 
	);
}

bool PathNodeManager::write(vector<Waypoint*> network, const char *file)
{
	TiXmlDocument *doc = new TiXmlDocument();

	for (unsigned int i=0; i<network.size(); i++)	
	{
		TiXmlElement *node = new TiXmlElement("node");
		node->SetAttribute("idx", i);
		
		for (unsigned int k=0; k<network[i]->con.size(); k++)
		{
			TiXmlElement *rel = new TiXmlElement("rel");
			int idx;

			// Find the index of the node
			for (unsigned int j=0; j<network.size(); j++)
			{
				if (network[j] == network[i]->con[k].wpt)
				{
					idx = j;
				}
			}

			int dst = (network[idx]->pos-network[i]->pos).length();
			
			rel->SetAttribute("dst", dst);
			rel->SetAttribute("idx", idx);
			node->LinkEndChild(rel);
		}

		stringstream ss;
		ss << network[i]->pos.x <<" " <<network[i]->pos.y;
		node->SetAttribute("pos", ss.str().c_str());

		doc->LinkEndChild(node);
	}

	return doc->SaveFile(file);
}