/***

Point: Defines multiple classes for describing positions in space

***/

#ifndef POINT_H
#define POINT_H

class Point2D
{
public:

	double x;
	double y;

	Point2D();
	Point2D( double x, double y );
};

class Point2F
{
public:

	float x;
	float y;

	Point2F();
	Point2F( float x, float y );
};

class Point3F
{
public:

	float x;
	float y;
	float z;

	Point3F( float x, float y, float z );
	Point3F();
};

class Point3D
{
public:

	double x;
	double y;
	double z;

	Point3D( double x, double y, double z );
	Point3D();

	void equal( double x, double y, double z );
	Point3D static add( Point3D p1, Point3D p2 );
	Point3D static sub( Point3D p1, Point3D p2 );
};

class Point3I
{
public:

	int x;
	int y;
	int z;

	Point3I( int x, int y, int z );
	Point3I();
};

class Point4F
{
public:

	float x;
	float y;
	float z;
	float w;

	Point4F( float x, float y, float z, float w );
	Point4F();
};

class Point4D
{
public:

	double x;
	double y;
	double z;
	double w;

	Point4D( double x, double y, double z, double w );
	Point4D();
};

#endif // !POINT_H
