/***

Static Object: A display object that does not have any animation

***/

#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H

#include "Metrics.h"
#include "Math.h"
#include "DisplayObject.h"
#include "WorldObject.h"
#include "Model.h"

class StaticObject : public DisplayObject, public WorldObject
{
protected:
	// The model of the object
	Model* objectModel;

	// Position in world coordinates
	Point3D position;

	// Flag for keeping aspect ratio of model
	bool aspectRatio;

	// The scale of the object
	Size3D scale;
	Size3D size;
	Size3D worldSize;

	// The rotation of the object
	Rotation3D rotation;

	// The movement velocity of the object
	Vector3D moveVelocity;

	// The rotation velocity of the object
	Vector3D rotateVelocity;

	// The tint of the object
	Color4F tint;

	// Override texture of the material
	GLuint overrideTexture;

	// flat shade vs smooth shade
	bool smoothShade;

	// Times to control frame rate independent movement
	Timer moveTimer;
	Timer eventTimer;

public:

	StaticObject();

	// Sets the model of the object
	bool setModel( Model* newModel );
	Model* getModel();

	// Set an override texture for the object
	// -This will replace EVERY material texture for the object!
	virtual void SetTexture( GLuint newTexture );

	// Changes OpenGL states for the current object
	//  -Returns false if the model should not be drawn
	virtual bool PrepareDraw();

	// Draws the object 
	virtual void draw();

	// Moves the object
	virtual void move();

	// Handles events for objects
	virtual void handleEvent( SDL_Event& e ) {};

	virtual void Pause();
	virtual void Unpause();

	// Draws debug information
	void drawDebugInfo( double length = 0.2 );

	// Position mutators
	virtual void setPosition( double x, double y, double z );
	virtual void setPosition( Point3D position );
	virtual void addToPosition( double x, double y, double z );
	Point3D GetPosition();

	// Size and scale mutators
	void setMaintainAspectRatio( bool flag );
	virtual void setSize ( double w, double h, double d );
	virtual void setScale( double w, double h, double d );
	virtual void addToScale( double w, double h, double d );
	Size3D GetSize();

	// Rotation mutators
	virtual void setRotation( double x, double y, double z );
	virtual void setRotation( Rotation3D rotation );
	virtual void addToRotation( double x, double y, double z );

	// Move velocity mutators
	virtual void setVelocity( double x, double y, double z );
	virtual void addToVelocity( double x, double y, double z );

	// Rotation velocity mutators
	virtual void setRotationVelocity( double x, double y, double z );
	virtual void addToRotationVelocity( double x, double y, double z );

	// Visual mutators
	virtual void setTint( float r, float g, float b, float a );
	void setSmoothShading( bool smooth );

};

#endif // !STATIC_OBJECT_H
