#include "Face.h"

Face::Face()
{
	verts[0] = NULL;
	verts[1] = NULL;
	verts[2] = NULL;
	verts[3] = NULL;
	material = "";
}

Face::Face( Vertex* v1, Vertex* v2, Vertex* v3, Point2D* t1, Point2D* t2, Point2D* t3, std::string material )
{
	verts[0] = v1;
	verts[1] = v2;
	verts[2] = v3;
	textures[0] = t1;
	textures[1] = t2;
	textures[2] = t3;
	this->material = material;
}

Point3D Face::GetPosition()
{
	return position;
}

Size3D Face::GetSize()
{
	return size;
}