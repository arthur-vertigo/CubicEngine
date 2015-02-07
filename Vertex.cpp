#include "Vertex.h"

Vertex::Vertex()
{
	texture.x = 1.0;
	texture.y = 1.0;

	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

	position.x = -1;
	position.y = -1;
	position.z = -1;

	normal.x = -1;
	normal.y = -1;
	normal.z = -1;
}

Vertex::Vertex( double x, double y, double z )
{
	position.x = x;
	position.y = y;
	position.z = z;
}

Vertex::Vertex( double x, double y, double z, double nX, double nY, double nZ )
{
	position.x = x;
	position.y = y;
	position.z = z;
}