#include "AnimationContainer.h"

std::unordered_map<std::string,Animation*> AnimationContainer::animations;
Animation* AnimationContainer::lastAdded = NULL;

Animation* AnimationContainer::NewAnimation( std::string name )
{
	if( name.length() == 0 )
		return NULL;
	
	if( HasAnimation( name ) )
		return animations[name];

	animations[name] = new Animation;
	lastAdded = animations[name];

	return animations[name];
}

bool AnimationContainer::AddFrameToAnimation( std::string name, Model* model, Size3D* size, Rotation3D* rotation, Point3D* position )
{
	if( !HasAnimation( name ) )
		return false;

	animations[name]->SetFrame( model, size, rotation, position );
	return true;
}

bool AnimationContainer::AddFrameToAnimation( Model* model, Size3D* size, Rotation3D* rotation, Point3D* position )
{
	if( lastAdded == NULL )
		return false;

	int prevFrame = lastAdded->AmountOfFrames()-1;

	if( model == NULL )
		model = lastAdded->GetFrameModel( prevFrame );

	if( size == NULL )
		size = lastAdded->GetFrameSize( prevFrame );

	if( rotation == NULL )
		rotation = lastAdded->GetFrameRotation( prevFrame );

	if( position == NULL )
		position = lastAdded->GetFramePoint( prevFrame );

	lastAdded->SetFrame( model, size, rotation, position );
	return true;
}

bool AnimationContainer::DoubleFramesAndReverseTail( std::string name )
{
	if( !HasAnimation( name ) )
		return false;

	animations[name]->DoubleFramesAndReverseTail();
	return true;
}

bool AnimationContainer::DoubleFramesAndReverseTail()
{
	if( lastAdded == NULL )
		return false;

	lastAdded->DoubleFramesAndReverseTail();
	return true;
}

Animation* AnimationContainer::LoadBlenderAnimationSequence( std::string path, std::string name )
{
	if( HasAnimation( name ) )
		return animations[name];

	NewAnimation( name );

	int frame = 1;
	std::string modelName = name;
	// Remove the tailing blender formating if detected
	StringFunctions::spliceOut( path, "_000001.obj" );
	std::string modelString = path;

	std::stringstream ss;
	ss << modelString << "_000001.obj"; // First model in the sequence

	std::stringstream ss2;
	ss2 << modelName << frame;

	// Keep going until there is a failed model load ( end of sequence )
	while( ModelContainer::addModel( ss.str(), ss2.str(), true, false ) )
	{
		// Push the loaded model into the animation 
		AddFrameToAnimation( name, ModelContainer::getModel( ss2.str() ) );

		frame++;

		// Add the corrent amount of tail characters for the next model in the sequence
		// - This only supports up to 999999 frames ... however; thats a freaking lot of frames. Would most
		//   likely run out of memory before frames
		ss.str("");
		if( frame < 10 )
		{
			ss << modelString << "_00000" << frame << ".obj";
		}
		else
		if( frame < 100 )
		{
			ss << modelString << "_0000" << frame << ".obj";
		}
		else
		if( frame < 1000 )
		{
			ss << modelString << "_000" << frame << ".obj";
		}
		else
		if( frame < 10000 )
		{
			ss << modelString << "_00" << frame << ".obj";
		}
		else
		if( frame < 100000 )
		{
			ss << modelString << "_0" << frame << ".obj";
		}
		else
		{
			ss << modelString << "_" << frame << ".obj";
		}

		// Set the next name 
		ss2.str("");
		ss2 << modelName << frame;
	}

	if( frame == 1 )
	{
		delete animations[name];
		return NULL;
	}

	return animations[name];
}

Animation* AnimationContainer::GetAnimation( std::string name )
{
	if( !HasAnimation( name ) )
		return NULL;

	return animations[name];
}

bool AnimationContainer::HasAnimation( std::string name )
{
	if( animations.find(name) == animations.end() )
		return false;
	return true;
}

void AnimationContainer::Clear( std::string compare )
{
}