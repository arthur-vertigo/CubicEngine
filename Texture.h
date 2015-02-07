/***

Texture: Defines a texture

***/

#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_opengl.h"

#include "Metrics.h"

class Texture
{
public:
	GLuint textureID; // OpenGL bound UINT to the texture
	Size2I textureSize; // Size of the texture
	Texture();
};

#endif // !TEXTURE_H
