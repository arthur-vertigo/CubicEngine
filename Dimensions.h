/***

Size: Defines size objects
Rotation: Defines rotation objects

***/

#ifndef DIMENSIONS_H
#define DIMENSIONS_H

// Size Class

class Size2I
{
public:
	int w;
	int h;

	Size2I();
	Size2I( int w, int h );
};

class Size2F
{
public:
	float w;
	float h;

	Size2F();
	Size2F( float w, float h );
};

class Size2D
{
public:
	double w;
	double h;

	Size2D();
	Size2D( double w, double h );
};

class Size3F
{
public:
	float w;
	float h;
	float d;

	Size3F();
	Size3F( float w, float h, float d );
};

class Size3D
{
public:
	double w;
	double h;
	double d;

	Size3D();
	Size3D( double w, double h, double d );
};

// Rotation Class

class Rotation2F
{
public:
	float x;
	float y;

	Rotation2F();
	Rotation2F( float x, float y );

	void keepInRange();
};

class Rotation2D
{
public:
	double x;
	double y;

	Rotation2D();
	Rotation2D( double x, double y );

	void keepInRange();
};

class Rotation3F
{
public:
	float x;
	float y;
	float z;

	Rotation3F();
	Rotation3F( float x, float y, float z );

	void keepInRange();
};

class Rotation3D
{
public:
	double x;
	double y;
	double z;

	Rotation3D();
	Rotation3D( double x, double y, double z );

	void keepInRange();
};  
#endif // !DIMENSIONS_H
