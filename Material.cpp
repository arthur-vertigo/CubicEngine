#include "Material.h"	

Material::Material()
{
	name = "";
	diffuse.r = 1.f;
	diffuse.g = 1.f;
	diffuse.b = 1.f;
	diffuse.a = 0.0f;

	specularity.r = 0.5f;
	specularity.g = 0.5f;
	specularity.b = 0.5f;
	specularity.a = 1.0f;

	emissive.r = 0.0f;
	emissive.g = 0.0f;
	emissive.b = 0.0f;
	emissive.a = 1.0f;

	ambience.r = 0.0;
	ambience.g = 0.0;
	ambience.b = 0.0;
	ambience.a = 0.0;

	specularExponent = 64;
	dissolve = 0.f;
	illuminationModel = 0;
	texture = -1;
	textureName = "";
}