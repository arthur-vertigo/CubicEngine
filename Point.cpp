#include "Point.h"

// Point 2F

Point2F::Point2F()
{
	x = 0.f;
	y = 0.f;
}
Point2F::Point2F( float x, float y )
{
	this->x = x;
	this->y = y;
}

// Point 2D

Point2D::Point2D()
{
	x = 0.0;
	y = 0.0;
}

Point2D::Point2D( double x, double y )
{
	this->x = x;
	this->y = y;
}

Point3F::Point3F()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

Point3F::Point3F( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// Point 3D

Point3D::Point3D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Point3D::Point3D( double x, double y, double z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Point3D Point3D::add( Point3D p1, Point3D p2 )
{
	Point3D returnPoint( p1.x + p2.x, p1.x + p2.x, p1.z + p2.z );
	return returnPoint;
}

void Point3D::equal( double x, double y, double z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Point3D sub( Point3D p1, Point3D p2 )
{
	Point3D returnPoint( p1.x - p2.x, p1.x - p2.x, p1.z - p2.z );
	return returnPoint;
}

// Point 3I

Point3I::Point3I()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point3I::Point3I( int x, int y, int z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// Point 4F

Point4F::Point4F()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	w = 0.f;
}

Point4F::Point4F( float x, float y, float z, float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

// Point 4D

Point4D::Point4D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 0.0;
}

Point4D::Point4D( double x, double y, double z, double w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}