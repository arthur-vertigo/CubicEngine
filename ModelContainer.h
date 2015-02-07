/***

Model Container: Contains loaded models

***/

#ifndef MODEL_CONTAINER_H
#define MODEL_CONTAINER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
#include "EngineObject.h"
#include "Model.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

class ModelContainer : public EngineObject
{
private:
	static std::unordered_map<std::string,Model*> GLTextures;
public:

	// Will create a new model and load the .obj file at path
	// -Associates name with the model
	// -Allow incomplete loading will load a model if there is missing texture data
	static bool addModel( std::string path, std::string name, bool allowIncompleteLoading = false, bool sizeAndCenter = true );

	// Will add the specifed model to the model container
	static bool addModel( Model* newModel, std::string name );

	// Returns a ptr to the model associated with name
	static Model* getModel( std::string name );

	// Returns true if the model is in the container
	static bool HasModel( std::string name );

	// Clears out the model container
	// -Deletes all the the models
	// -Optional string for comparision
	//  -Only deletes strings the pass the comparision test
	static void clear( std::string compare = "" );

	// Clears out all of the VBOs and IBOs bound;
	static void ClearVBO_IBOs();

	// Will serilalize all models in the model container
	// - Accepts a comparision string to filter the models exported
	static bool ExportModels( std::string path, std::string compare = "" );
};  

#endif // !MODEL_CONTAINER_H
