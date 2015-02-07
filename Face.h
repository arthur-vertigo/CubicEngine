/***

Face: Defines one triangular or quadrilateral face.
       -Holds the location of the vertices, texture position, and material

***/

#ifndef FACE_H
#define FACE_H

#include "Point.h"
#include "Vector.h"
#include "Vertex.h"
#include "Material.h"
#include "WorldObject.h"
#include <string>

class Face : public WorldObject
{
public:
	Face();
	Face( Vertex* v1, Vertex* v2, Vertex* v3, Point2D* t1, Point2D* t2, Point2D* t3, std::string material );
	Vertex* verts[4];
	Point2D* textures[4];
	std::string material;

	Point3D position;
	Size3D size;

	Point3D GetPosition();
	Size3D GetSize();
};

#endif // !FACE_H
