/***

OctTree: 

***/


#ifndef OCT_TREE_H
#define OCT_TREE_H

#include <vector>
#include "Metrics.h"
#include "WorldObject.h"

class OctTree
{
private:
	bool InBounds( Point3D position, Size3D size, Point3D point );
	bool OverlapArea( Point3D positionA, Size3D sizeA, Point3D positionB, Size3D sizeB );
	void Split();
public:
	OctTree();
	OctTree( double x, double y, double z, double w, double h, double d );
	Point3D position;
	Size3D size;

	int length;
	int amountOfChildren;
	OctTree* nodes;
	WorldObject** data;

	bool Insert( WorldObject* data );
	std::vector< WorldObject* > GetObjects( Point3D position, Size3D size );

	void clear();
};  

#endif // !OCT_TREE_H
