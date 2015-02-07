/***

Container class for GLuint texture ints

***/

#ifndef GL_TEXTURE_CONTAINER
#define GL_TEXTURE_CONTAINER

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

#include "Texture.h"

class GLTextureContainer
{
private:
	static std::unordered_map<std::string,Texture*> GLTextures;
public:

	// Loads a texture from located at path, associates the texture with name
	static Texture* addTexture( std::string path, std::string name );

	// Returns the gluint texture associated with name
	// -Returns 0 if the texture is not found
	static Texture* getTexture( std::string name );

	// Returns true if the texture is in the container
	static bool hasTexture( std::string name );

	// Clears out the container
	// -Deletes all objects
	static void clear();
};  

#endif // !GL_TEXTURE_CONTAINER
