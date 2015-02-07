/***

Environment: Controls OpenGL fog and ambient lighting
			  -Will control up to one GL_LIGHT to simulate Sun / Moon / Planet, etc

***/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "Utilities.h"
#include "Metrics.h"
#include "Math.h"

// Defines a state that the environment seeks to become
class EnvironmentState
{
private:
public:
	EnvironmentState();

	double duration;
	Color3F skyColor;
	Color3F fogColor;
	double fogExp;
	Point3D lightPosition;
	Color3F lightColor;
};

class Environment
{
private:
	// Environment states
	std::vector<EnvironmentState*> envStates;

	// Current state (subscript of the vector)
	int currentState;
	int nextState;

	// Environment state change rate (seconds )
	double changeRate;
	double fogExpRate;
	Color3F skyColorChangeRate;
	Color3F fogColorChangeRate;
	Color3F lightColorChangeRate;
	Point3D lightPositionChangeRate;

	double currentFogExp;
	Color3F currentSkyColor;
	Color3F currentFogColor;

	GLenum light;
	Color3F currentLightColor;
	Point3D currentlightPosition;

	Point3D viewPosition;

	// ChangeTimer
	Timer changeTimer;
	Timer currentStateTimer;

	bool automaticStateChange;
	bool noChange;
	bool hasInit;
public:
	Environment();
	void Update(  Point3D* viewPosition = NULL );
	int GetCurrentState();


	void NewState();
	void SetDuration( double duration );
	void SetSkyColor( Color3F skyColor );
	void SetFogColor( Color3F fogColor );
	void SetFogExponent( double fogExp );
	void SetLightColor( Color3F lightColor );
	void SetLightPosition( Point3D lightPosition );

	void SetControllableLight( GLenum lightEnum );
	Point3D GetLightPosition();

	void AddState( int duration, Color3F skyColor, Color3F fogColor, double fogExp );

	void Pause();
	void Unpause();

	void SetState( int state, bool force = true  );
	void SetAutomaticStateChange( bool flag );
};

#endif // !ENVIRONMENT_H
