/***

Animation: Stores a series of model pointers which forms an animation
Animated Object: Stores a collection of models which defines frames of an animation

***/

#ifndef ANIMATED_OBJECT_H
#define ANIMATED_OBJECT_H

#include <vector>

#include "StaticObject.h"
#include "DisplayObject.h"
#include "WorldObject.h"
#include "Model.h"
#include "Animation.h"

class AnimatedObject : public StaticObject
{
private:
	int frame;
	int frameMax;
	int frameMin;
	int fps;
	bool playing;
	bool looping;
	Animation* animation;
	Timer fpsTimer;

public:
	AnimatedObject();

	void Play();
	void Stop();

	void SetFrameRange( int min, int max = 0 );
	void SetLooping( bool flag );
	void GotoFrame( int frame );
	void GotoRandomFrame();

	void draw();

	virtual void Pause();
	virtual void Unpause();

	void SetFPS( int fps );
	void SetAnimation( Animation* animation );

	// Returns true if the frame has changed
	bool IncrementFrame();
};  
#endif // !ANIMATED_OBJECT_H
