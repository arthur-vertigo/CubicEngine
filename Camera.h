/***

Camera: Controls the viewport of the application

***/

#ifndef CAMERA_H
#define CAMERA_H

#include "Metrics.h"
#include "DisplayObject.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
#include "GL_Debug.h"
#include "Math.h"

class Camera
{
private:

	// Demo. information about camera
	Point3D position;
	Rotation3D rotation;
	Vector3F up;

	// The current view camera, used in controlling the opengl matrix system
	static Camera* currentCamera;
public:
	Camera();

	// Resets the current view camera 
	//  -This allows the use of multiple cameras in one rendering scene
	static void resetCamera();

	// Draws the position of the camera in world coords
	void draw();

	// Sets the screen to view from this camera
	void setView();

	// Adds x,y,z to the position of the camera
	void addToPosition( double x, double y, double z );

	// Sets the position of the camera to x,y,z
	void setPosition( double x, double y, double z );
	void setPosition( Point3D position );

	// Returns the position of the camera
	Point3D getPosition();

	// Adds to the rotation
	void addToRotation( double x, double y, double z );

	// Sets the rotation of the camera to x,y,z
	void setRotation( double x, double y, double z );
	void setRotation( Rotation3D rotation );

	// Gets the rotation of the camera
	Rotation3D getRotation();	
};

#endif // !CAMERA_H
