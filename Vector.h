/***

Vector: Defines a direction of movement

***/

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "sdl.h"
#include <cmath>

class Vector2F
{
public:
	Vector2F();
	Vector2F( float x, float y, float mag = 1.f );

	float x;
	float y;
	float mag;

	void Normalize();
	void clear();
};

class Vector2D
{
public:
	Vector2D();
	Vector2D( double x, double y, double mag = 1.0 );

	double x;
	double y;
	double mag;

	void Normalize();
	void clear();
};

class Vector3F
{
public:
	Vector3F();
	Vector3F( float x, float y, float z, float mag = 1.f );

	float x;
	float y;
	float z;
	float mag;

	void Normalize();
	void clear();
};

class Vector3D
{
public:
	Vector3D();
	Vector3D( double x, double y, double z, double mag = 1.0 );

	double x;
	double y;
	double z;
	double mag;

	void equals( double x, double y, double z, double mag );

	void Normalize();
	void clear();
};

class Vector4F
{
public:
	Vector4F();
	Vector4F( float x, float y, float z, float w, float mag = 1.f );

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

#endif // !VECTOR3D_H
