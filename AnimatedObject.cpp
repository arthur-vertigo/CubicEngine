#include "AnimatedObject.h"

//
// Animated Object
//


AnimatedObject::AnimatedObject()
{
	frame = 0;
	frameMax = 0;
	frameMin = 0;
	fps = 1;
	playing = false;
	looping = false;
	animation = NULL;
	fpsTimer.start();
}

void AnimatedObject::Play()
{
	if( animation == NULL )
		return;

	playing = true;
}

void AnimatedObject::Stop()
{
	playing = false;
}

void AnimatedObject::SetFrameRange( int min, int max )
{
	if( min < 0 || animation == NULL )
		return;

	if( max == 0 )
		max = animation->AmountOfFrames();

	if( min > max )
		min = max;

	if( max < min )
		max = min;

	frameMin = min;
	frameMax = max;

	if( frame < frameMin )
		frame = frameMin;

	if( frame > frameMax )
		frame = frameMin;
}

void AnimatedObject::SetLooping( bool flag )
{
	looping = flag;
}

void AnimatedObject::GotoFrame( int frame )
{
	if( frame < 0 || animation == NULL || frame > animation->AmountOfFrames() )
		return;

	this->frame = frame;
}

void AnimatedObject::GotoRandomFrame()
{
	GotoFrame( Math::RandomInt( frameMin, frameMax ) );
}

void AnimatedObject::SetFPS( int fps )
{
	if( fps < 0 )
		return;

	this->fps = fps;
}

void AnimatedObject::SetAnimation( Animation* animation )
{
	if( animation == NULL || animation->AmountOfFrames() <= 0 )
		return;

	this->animation = animation;

	if( frameMax > animation->AmountOfFrames() )
		frameMax = animation->AmountOfFrames();

	this->setModel( animation->GetFrameModel( frameMin ) );

	this->setRotation( animation->GetFrameRotation( frameMin )->x, animation->GetFrameRotation( frameMin )->y, animation->GetFrameRotation( frameMin )->z );

	this->setPosition( animation->GetFramePoint( frameMin )->x, animation->GetFramePoint( frameMin )->y, animation->GetFramePoint( frameMin )->z );

	this->setScale( animation->GetFrameSize( frameMin )->w, animation->GetFrameSize( frameMin )->h, animation->GetFrameSize( frameMin )->d );
}

void AnimatedObject::draw()
{
	if( IncrementFrame() )
		this->setModel( animation->GetFrameModel( frame-1 ) );

	if( !StaticObject::PrepareDraw() || objectModel == NULL )
		return;

	// Move the player according to the animation parameters
	objectModel->draw(	Point3D( this->position.x + animation->GetFramePoint( frame-1 )->x, this->position.y + animation->GetFramePoint( frame-1 )->y, this->position.z + animation->GetFramePoint( frame-1 )->z ),
						Size3D( this->scale.w * animation->GetFrameSize( frame-1 )->w, this->scale.h * animation->GetFrameSize( frame-1 )->h, this->scale.d * animation->GetFrameSize( frame-1 )->d ),
						Rotation3D( this->rotation.x + animation->GetFrameRotation( frame-1 )->x, this->rotation.y + animation->GetFrameRotation( frame-1 )->y, this->rotation.z + animation->GetFrameRotation( frame-1 )->z ),
						tint,
						overrideTexture );
}

bool AnimatedObject::IncrementFrame()
{
	if( animation == NULL )
		return false;

	if( fpsTimer.get_ticks() > 1000.0 / fps )
	{
		frame++;

		if( frame > frameMax )
		{
			frame = frameMin;

			if( !looping )
				playing = false;
		}

		fpsTimer.start();
		return true;
	}
	return false;
}

void AnimatedObject::Pause()
{
	fpsTimer.pause();
	StaticObject::Pause();
}

void AnimatedObject::Unpause()
{
	fpsTimer.unpause();
	StaticObject::Unpause();
}