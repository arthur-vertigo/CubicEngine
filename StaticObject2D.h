/***

Static Object 2D: Defines a 2d static object ment for ortho rendering

***/

#ifndef STATIC_OBJECT_2D_H
#define STATIC_OBJECT_2D_H

#include "StaticObject.h"
#include "ModelContainer.h"
#include "DynamicMesh.h"
#include "Model.h"

class StaticObject2D : public StaticObject
{
private:
public:
	// Sets the x rotation to 90 to face the display object towards the camera
	StaticObject2D();
	StaticObject2D( Model* BarModel );

	// Draws the object 
	virtual void draw();

	//  Overloaded mutators
	//  -Considers d = h;
	void setSize ( double w, double h, double d );
	void setScale( double w, double h, double d );
	void addToScale( double w, double h, double d );

	void setRotation( double x, double y, double z );
	void addToRotation( double x, double y, double z );

	void setVelocity( double x, double y, double z );
	void addToVelocity( double x, double y, double z );

	void setRotationVelocity( double x, double y, double z );
	void addToRotationVelocity( double x, double y, double z );
};

#endif // !STATIC_OBJECT_2D_H
