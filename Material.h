/***

Material: Defines a 3d material

***/

#include "SDL.h"
#include "SDL_opengl.h"

#include "Metrics.h"
#include <string>

#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
public:
	// The name of the material
	std::string name;

	// The amount of spec 'spot' effect
	int specularExponent;

	// Dont know :/
	float dissolve;

	// The color of specularity light, 'spot' effect
	Color4F specularity;

	// Color, which in darkness, the object seems to produce
	Color4F ambience;

	// The color of the object
	Color4F diffuse;

	// Color that the object radiates
	Color4F emissive;

	// Not implimented
	int illuminationModel;

	// The texture int for the material
	GLuint texture;

	// The texture name for the material
	std::string textureName;

	Material();
};



#endif // !MATERIAL_H
