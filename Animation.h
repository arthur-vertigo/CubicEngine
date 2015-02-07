/***

Animation: Stores a sequence of models and transformations to simulate an animation

***/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

#include "Model.h"
#include "Metrics.h"

class Animation
{
private:
	std::vector<Model*> models;
	std::vector<Size3D*> sizes;
	std::vector<Rotation3D*> rotations;
	std::vector<Point3D*> positions;
	int amountOfFrames;

public:
	Animation();
	void SetFrame( Model* newFrameModel, Size3D* size = NULL, Rotation3D* rotation = NULL, Point3D* position = NULL );
	void DoubleFramesAndReverseTail();
	int AmountOfFrames();

	Model* GetFrameModel( int frame );
	Size3D* GetFrameSize( int frame );
	Rotation3D* GetFrameRotation( int frame );
	Point3D* GetFramePoint( int frame );
};  
#endif // !ANIMATION_H
