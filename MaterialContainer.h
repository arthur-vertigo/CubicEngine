/***

Container class for object materials

***/

#ifndef MATERIAL_CONTAINER
#define MATERIAL_CONTAINER

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"

#include "Material.h"
#include "GLTextureContainer.h"
#include "StringFunctions.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>

class MaterialContainer
{
private:
	static std::unordered_map<std::string,Material*> materials;
	static Material* lastMaterial;
public:

	// Loads a material from located at path, associates the texture with name
	static bool loadMaterialFile( std::string path );

	// Creates a new material
	static Material* NewMaterial( std::string name );

	// Sets the diffuse of the last material
	static void SetDiffuse( Color4F diffuse );

	// Sets the ambience of the last material
	static void SetAmbience( Color4F ambience );

	// Sets the emissive of the last material
	static void SetEmissive( Color4F emissive );

	// Sets the specularity of the last material
	static void SetSpecularity( Color4F specularity );

	// Sets the specularity exponent
	static void SetSpecularityExponent( int specExponent );

	// Sets the texture of the material
	static void SetTexture( Texture* texture, std::string name = "" );
	// -Associated with GLTextureContainer
	static void SetTexture( std::string textureName );

	// Inserts a material already loaded
	static void addMaterial( Material* material, std::string name  );

	// Returns the material associated with name
	// -Returns NULL if the material is not found
	static Material* getMaterial( std::string name );

	// Returns true if the material is in the container
	static bool hasMaterial( std::string name );

	// Clears out the container
	// -Deletes all objects
	static void clear();

	// Exports all materials
	static bool ExportMaterials( std::string path );
};  

#endif // !MATERIAL_CONTAINER
