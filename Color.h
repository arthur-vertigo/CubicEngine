/***

Color: Defines either a RGB or a RGBA color

***/

#ifndef COLOR_H
#define COLOR_H

typedef unsigned char byte;

class Color3B
{
public:
	byte r;
	byte g;
	byte b;

	Color3B();
	Color3B( byte r, byte g, byte b );

};

class Color3F
{
public:
	float r;
	float g;
	float b;

	Color3F();
	Color3F( float r, float g, float b );
};

class Color4F
{
public:
	float r;
	float g;
	float b;
	float a;

	Color4F();
	Color4F( float r, float g, float b, float a );
};

#endif // !COLOR_H
