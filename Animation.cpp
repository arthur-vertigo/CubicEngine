#include "Animation.h"

//
// Animation
//

Animation::Animation()
{
	amountOfFrames = 0;
}

void Animation::SetFrame( Model* newFrameModel, Size3D* size, Rotation3D* rotation, Point3D* position )
{
	if( newFrameModel == NULL )
		return;

	models.push_back( newFrameModel );

	if( size != NULL )
		sizes.push_back( new Size3D( size->w, size->h, size->d ) );
	else
		sizes.push_back( new Size3D( 1.0, 1.0, 1.0 ) );

	if( rotation != NULL )
		rotations.push_back( new Rotation3D( rotation->x, rotation->y, rotation->z ) );
	else
		rotations.push_back( new Rotation3D( 0.0, 0.0, 0.0 ) );

	if( position != NULL )
		positions.push_back( new Point3D( position->x, position->y, position->z ) );
	else
		positions.push_back( new Point3D( 0.0, 0.0, 0.0 ) );

	amountOfFrames++;
}

void Animation::DoubleFramesAndReverseTail()
{
	for( int i = models.size()-2; i >= 0; i-- )
	{
		models.push_back( models[i] );
		sizes.push_back( sizes[i] );
		rotations.push_back( rotations[i] );
		positions.push_back( positions[i] );
		amountOfFrames++;
	}
}

int Animation::AmountOfFrames()
{
	return amountOfFrames;
}

Model* Animation::GetFrameModel( int frame )
{
	if( frame < 0 || frame > static_cast<int>( models.size()-1 ) )
		return NULL;
	return models[frame];
}

Size3D* Animation::GetFrameSize( int frame )
{
	if( frame < 0 || frame > static_cast<int>( models.size()-1 ) )
		return NULL;
	return sizes[frame];
}

Rotation3D* Animation::GetFrameRotation( int frame )
{
	if( frame < 0 || frame > static_cast<int>( models.size()-1 ) )
		return NULL;
	return rotations[frame];
}

Point3D* Animation::GetFramePoint( int frame )
{
	if( frame < 0 || frame > static_cast<int>( models.size()-1 ) )
		return NULL;
	return positions[frame];
}
