#include "Dimensions.h"

/////////////////
////// Size Class
/////////////////

// Size 2I

Size2I::Size2I()
{
	w = 0;
	h = 0;
}

Size2I::Size2I( int w, int h )
{
	this->w = w;
	this->h = h;
}

// Size 2F

Size2F::Size2F()
{
	w = 0.f;
	h = 0.f;
}

Size2F::Size2F( float w, float h )
{
	this->w = w;
	this->h = h;
}

// Size 2D

Size2D::Size2D()
{
	w = 0.0;
	h = 0.0;
}

Size2D::Size2D( double w, double h )
{
	this->w = w;
	this->h = h;
}

// Size 3F

Size3F::Size3F()
{
	w = 0.f;
	h = 0.f;
	d = 0.f;
}

Size3F::Size3F( float w, float h, float d )
{
	this->w = w;
	this->h = h;
	this->d = d;
}

// Size 3D

Size3D::Size3D()
{
	w = 0.0;
	h = 0.0;
	d = 0.0;
}

Size3D::Size3D( double w, double h, double d )
{
	this->w = w;
	this->h = h;
	this->d = d;
}

/////////////////
///Rotation Class
/////////////////

//Rotation 2F

Rotation2F::Rotation2F()
{
	x = 0.f;
	y = 0.f;
}

Rotation2F::Rotation2F( float x, float y )
{
	this->x = x;
	this->y = y;
}

void Rotation2F::keepInRange()
{
	while( x > 360.f )
		x-= 360.f;

	while( y > 360.f )
		y-= 360.f;

}

//Rotation 2D

Rotation2D::Rotation2D()
{
	x = 0.0;
	y = 0.0;
}

Rotation2D::Rotation2D( double x, double y )
{
	this->x = x;
	this->y = y;
}

void Rotation2D::keepInRange()
{
	while( x > 360.0 )
		x-= 360.0;

	while( y > 360.0 )
		y-= 360.0;

}

//Rotation 3F

Rotation3F::Rotation3F()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

Rotation3F::Rotation3F( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Rotation3F::keepInRange()
{
	while( x > 360.f )
		x-= 360.f;

	while( y > 360.f )
		y-= 360.f;

	while( z > 360.f )
		z-= 360.f;

	while( x < -360.f )
		x += 360.f;

	while( y < -360.f )
		y += 360.f;

	while( z < -360.f )
		z += 360.f;
}

//Rotation 3D

Rotation3D::Rotation3D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Rotation3D::Rotation3D( double x, double y, double z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Rotation3D::keepInRange()
{
	while( x > 360.0 )
		x-= 360.0;

	while( y > 360.0 )
		y-= 360.0;

	while( z > 360.0 )
		z-= 360.0;

	while( x < -360.0 )
		x += 360.0;

	while( y < -360.0 )
		y += 360.0;

	while( z < -360.0 )
		z += 360.0;
}