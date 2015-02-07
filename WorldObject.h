/***

World Object: Defines an object that had world properties and methods

***/

#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "Metrics.h"

class WorldObject
{
public:
	virtual Point3D GetPosition() = 0;
	virtual Size3D GetSize() = 0;
};

#endif // !WORLD_OBJECT_H
