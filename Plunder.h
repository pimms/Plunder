#pragma once

#include "Pim.h"
#include "Box2D\Box2D.h"


#ifndef PLUNDER_DEFS
#define PLUNDER_DEFS

	// Pixel to Meter-ratio
	#define PTMR 32.f

	// Collision Flags
	#define CF_WALLS		1
	#define CF_LOOT			2
	#define CF_LEVER		4
	#define CF_UNDERGROUND	8
	#define CF_WAYPOINT		16
	#define CF_PLAYER		32
	#define CF_GUARD		64
	#define CF_INTERACTIVE	(CF_LEVER|CF_UNDERGROUND|CF_LOOT|CF_BANDITCHEST)
	#define CF_BANDITCHEST	128
	
	// Direction definitions
	#define D_UP	0
	#define D_RIGHT 1
	#define D_DOWN	2
	#define D_LEFT	3

#endif /*PLUNDER_DEFS*/
// Pim::Vec2 to b2Vec2 conversion. PTM-Ratio is taken into account.
inline b2Vec2 toB2(const Pim::Vec2 &v)
{
	return b2Vec2(v.x / PTMR, v.y / PTMR);
}

// b2Vec2 to Pim::Vec2 conversion. PTM-Ratio is taken into account.
inline Pim::Vec2 toPim(const b2Vec2 &v)
{
	return Pim::Vec2(v.x * PTMR, v.y * PTMR);
}