/***

Vertex: Defines a position on a model
        -Contains the position and normal for the vertex

***/

#ifndef VERTEX_H
#define VERTEX_H

#include "Vector.h"
#include "Point.h"
#include "Metrics.h"
#include "Color.h"


class Vertex
{
public:
	Vertex();
	Vertex( double x, double y, double z );
	Vertex( double x, double y, double z, double nX, double nY, double nZ );
	Point3D position;
	Vector3D normal;
	Point2F texture;
	Color4F color;
};

#endif // !VERTEX_H
