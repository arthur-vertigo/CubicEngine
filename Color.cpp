#include "Color.h"

// Color 3b

Color3B::Color3B()
{
	r = 0;
	g = 0;
	b = 0;
}

Color3B::Color3B( byte r, byte g, byte b )
{
	this->r = r;
	this->g = g;
	this->b = b;
}

// Color 3F

Color3F::Color3F()
{
	r = 0.f;
	g = 0.f;
	b = 0.f;
}

Color3F::Color3F( float r, float g, float b )
{
	this->r = r;
	this->g = g;
	this->b = b;
}

// Color 4F

Color4F::Color4F()
{
	r = 0.f;
	g = 0.f;
	b = 0.f;
	a = 1.f;
}

Color4F::Color4F( float r, float g, float b, float a )
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
