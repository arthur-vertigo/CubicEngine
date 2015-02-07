/***

Animation Container: Creates and stores animations

***/

#ifndef ANIMATION_CONTAINER_H
#define ANIMATION_CONTAINER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"

#include "StringFunctions.h"
#include "ModelContainer.h"
#include "EngineObject.h"
#include "Animation.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

class AnimationContainer : public EngineObject
{
private:
	static std::unordered_map<std::string,Animation*> animations;
	static Animation* lastAdded;
public:

	// Will create a new animation
	static Animation* NewAnimation( std::string name );

	// Will add the animation + transformations frame to the tail of the animation with key name
	static bool AddFrameToAnimation( std::string name, Model* model = NULL, Size3D* size = NULL, Rotation3D* rotation = NULL, Point3D* position = NULL );
	// Will add the frame to the last created animation
	static bool AddFrameToAnimation( Model* model = NULL, Size3D* size = NULL, Rotation3D* rotation = NULL, Point3D* position = NULL );

	// Will double the amount of frames ( minue the last frame ) and reverse the order of the second half of the animation
	static bool DoubleFramesAndReverseTail( std::string name );
	static bool DoubleFramesAndReverseTail();

	// Will load a sequence of .obj files (named with blenders export convention ) into a single animation
	static Animation* LoadBlenderAnimationSequence( std::string path, std::string name );

	// Returns a ptr to the animation associated with name
	static Animation* GetAnimation( std::string name );

	// Returns true if the animation is in the container
	static bool HasAnimation( std::string name );

	// Clears out the animation container
	// -Deletes all the the animation
	// -Optional string for comparision
	//  -Only deletes strings the pass the comparision test
	static void Clear( std::string compare = "" );
};  

#endif // !ANIMATION_CONTAINER_H
