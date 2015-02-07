#include "Vector.h"

// Vector 2F

Vector2F::Vector2F()
{
	x = 0.f;
	y = 0.f;
	mag = 1.f;
}

Vector2F::Vector2F( float x, float y, float mag )
{
	this->x = x;
	this->y = y;
	this->mag = mag;
}

void Vector2F::Normalize()
{
	mag = sqrt( ( x * x ) + ( y * y ) );
	x /= mag;
	y /= mag;
	mag = 1.f;
}

void Vector2F::clear()
{
	x = 0.f;
	y = 0.f;
	mag = 0.f;
}

// Vector 2D

Vector2D::Vector2D()
{
	x = 0.0;
	y = 0.0;
	mag = 1.0;
}

Vector2D::Vector2D( double x, double y, double mag )
{
	this->x = x;
	this->y = y;
	this->mag = mag;
}

void Vector2D::Normalize()
{
	mag = sqrt( ( x * x ) + ( y * y ) );
	x /= mag;
	y /= mag;
	mag = 1.0;
}

void Vector2D::clear()
{
	x = 0.0;
	y = 0.0;
	mag = 0.0;
}

// Vector 3F

Vector3F::Vector3F()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	mag = 1.f;
}

Vector3F::Vector3F( float x, float y, float z, float mag )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->mag = mag;
}

void Vector3F::Normalize()
{
	mag = sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
	x /= mag;
	y /= mag;
	z /= mag;
	mag = 1.f;
}

void Vector3F::clear()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	mag = 0.f;
}

// Vector 3D

Vector3D::Vector3D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	mag = 1.0;
}

Vector3D::Vector3D( double x, double y, double z, double mag )
{
	this->x = x;
	this->y = y;
	this->z = z;
	mag = mag;
}

void Vector3D::Normalize()
{
	mag = sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
	x /= mag;
	y /= mag;
	z /= mag;
	mag = 1.0;
}

void Vector3D::clear()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	mag = 0.0;
}

void Vector3D::equals( double x, double y, double z, double mag )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->mag = mag;
}

// Vector 4F

Vector4F::Vector4F()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	w = 0.f;
	mag = 1.f;
}

Vector4F::Vector4F( float x, float y, float z, float w, float mag )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	mag = mag;
}

void Vector4F::Normalize()
{
	mag = sqrt( ( x * x ) + ( y * y ) + ( z * z ) + ( w * w ) );
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
	mag = 1.f;
}

void Vector4F::clear()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	w = 0.f;
	mag = 0.f;
}

// Vector 4D

Vector4D::Vector4D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 0.0;
	mag = 1.0;
}

Vector4D::Vector4D( double x, double y, double z, double w, double mag )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	mag = mag;
}

void Vector4D::Normalize()
{
	mag = sqrt( ( x * x ) + ( y * y ) + ( z * z ) + ( w * w ) );
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
	mag = 1.0;
}

void Vector4D::clear()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 0.0;
	mag = 0.0;
}

/*

class Vector4F
{
public:
	Vector4F();
	Vector4F( float x, float y, float z, double w, float mag = 1.f );

	float x;
	float y;
	float z;
	float w;
	float mag;

	void Normalize();
	void clear();
};

class Vector4D
{
public:
	Vector4D();
	Vector4D( double x, double y, double z, double w, double mag = 1.0 );

	double x;
	double y;
	double z;
	double w;
	double mag;

	void Normalize();
	void clear();
};

*/