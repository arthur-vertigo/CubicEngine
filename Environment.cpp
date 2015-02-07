#include "Environment.h"

//
// Environment State
//

EnvironmentState::EnvironmentState()
{
	duration = 1;
	fogExp = 0.0;
}

//
// Environment
//

Environment::Environment()
{
	currentState = 0;
	nextState = 0;
	changeRate = 1;
	fogExpRate = 0;

	currentFogExp = 0.0;

	light = NULL;
	automaticStateChange = false;
	noChange = false;
	hasInit = false;
}

void Environment::Update( Point3D* viewPosition )
{
	if( !automaticStateChange || changeTimer.is_paused() )
		return;

	if( viewPosition != NULL )
	{
		this->viewPosition.x = viewPosition->x;
		this->viewPosition.y = viewPosition->y;
		this->viewPosition.z = viewPosition->z;
	}

	if( currentStateTimer.get_ticks() > changeRate )
	{
		SetState( nextState );
		return;
	}

	if( noChange && hasInit )
	{
		hasInit = true;
		return;
	}

	currentSkyColor.r += skyColorChangeRate.r * changeTimer.get_ticks();
	currentSkyColor.g += skyColorChangeRate.g * changeTimer.get_ticks();
	currentSkyColor.b += skyColorChangeRate.b * changeTimer.get_ticks();

	currentFogColor.r += fogColorChangeRate.r * changeTimer.get_ticks();
	currentFogColor.g += fogColorChangeRate.g * changeTimer.get_ticks();
	currentFogColor.b += fogColorChangeRate.b * changeTimer.get_ticks();

	currentFogExp += fogExpRate * changeTimer.get_ticks();

	if( light != NULL )
	{
		currentLightColor.r += lightColorChangeRate.r * changeTimer.get_ticks();
		currentLightColor.g += lightColorChangeRate.g * changeTimer.get_ticks();
		currentLightColor.b += lightColorChangeRate.b * changeTimer.get_ticks();	
	
		currentlightPosition.x += lightPositionChangeRate.x * changeTimer.get_ticks();
		currentlightPosition.y += lightPositionChangeRate.y * changeTimer.get_ticks();
		currentlightPosition.z += lightPositionChangeRate.z * changeTimer.get_ticks();
	}

	changeTimer.start();

	//Fog
	GLfloat fogColor[4] = { currentFogColor.r, currentFogColor.g, currentFogColor.b, 0.0};
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, static_cast<float>( currentFogExp ) );
	
	// Set Sky Color
	glClearColor( currentSkyColor.r, currentSkyColor.g, currentSkyColor.b, 0 );

	if( light != NULL )
	{
		if( currentlightPosition.y < 0 )
		{
			glDisable( light );
		}
		else
		{
			glEnable( light );
		}

		float lightpos[4] = { currentlightPosition.x + this->viewPosition.x, currentlightPosition.y + this->viewPosition.y, currentlightPosition.z + this->viewPosition.z, 1.f  };
		glLightfv( light, GL_POSITION, lightpos );

		float diff[4] = { currentLightColor.r, currentLightColor.g, currentLightColor.b, 1.f };
		glLightfv( light, GL_DIFFUSE, diff );
	}
}

int Environment::GetCurrentState()
{
	return currentState;
}

void Environment::SetControllableLight( GLenum light )
{
	this->light = light;
}

Point3D Environment::GetLightPosition()
{
	return Point3D( currentlightPosition.x + viewPosition.x, currentlightPosition.y + viewPosition.y, currentlightPosition.z + viewPosition.z );
}

void Environment::NewState()
{
	envStates.push_back( new EnvironmentState );

	// If the states are greater than 2 copy the previous last element
	if( envStates.size() > 1 )
	{
		SetDuration( envStates[envStates.size()-2]->duration );
		SetSkyColor( envStates[envStates.size()-2]->skyColor );
		SetFogColor( envStates[envStates.size()-2]->fogColor );
		SetFogExponent( envStates[envStates.size()-2]->fogExp );
		SetLightColor( envStates[envStates.size()-2]->lightColor );
		SetLightPosition( envStates[envStates.size()-2]->lightPosition );
	}
}

void Environment::SetDuration( double duration )
{
	if( envStates.size() == 0 )
		return;

	envStates.back()->duration = duration;
}

void Environment::SetSkyColor( Color3F skyColor )
{
	if( envStates.size() == 0 )
		return;

	envStates.back()->skyColor.r = skyColor.r;
	envStates.back()->skyColor.g = skyColor.g;
	envStates.back()->skyColor.b = skyColor.b;
}

void Environment::SetFogColor( Color3F fogColor )
{

	if( envStates.size() == 0 )
		return;

	envStates.back()->fogColor.r = fogColor.r;
	envStates.back()->fogColor.g = fogColor.g;
	envStates.back()->fogColor.b = fogColor.b;
}

void Environment::SetFogExponent( double fogExp )
{
	if( envStates.size() == 0 )
		return;

	envStates.back()->fogExp = fogExp;
}

void Environment::SetLightColor( Color3F lightColor )
{
	if( envStates.size() == 0 )
		return;

	envStates.back()->lightColor.r = lightColor.r;
	envStates.back()->lightColor.g = lightColor.g;
	envStates.back()->lightColor.b = lightColor.b;
}

void Environment::SetLightPosition( Point3D lightPosition )
{
	if( envStates.size() == 0 )
		return;

	envStates.back()->lightPosition.x = lightPosition.x;
	envStates.back()->lightPosition.y = lightPosition.y;
	envStates.back()->lightPosition.z = lightPosition.z;
}

void Environment::AddState( int duration, Color3F skyColor, Color3F fogColor, double fogExp )
{
	EnvironmentState* newState = new EnvironmentState;
	newState->duration = duration;
	newState->skyColor = skyColor;
	newState->fogColor = fogColor;
	newState->fogExp = fogExp;
	envStates.push_back( newState );
}

void Environment::SetState( int state, bool force )
{
	if( state < 0 || state > static_cast<int>( envStates.size() ) )
		return;

	currentState = state;

	if( currentState+1 >= static_cast<int>( envStates.size() ) )
	{
		nextState = 0;
	}
	else
	{
		nextState = currentState+1;
	}

	if( force ) // Set the state RIGHT NOW!
	{

		currentSkyColor.r = envStates[currentState]->skyColor.r;
		currentSkyColor.g = envStates[currentState]->skyColor.g;
		currentSkyColor.b = envStates[currentState]->skyColor.b;

		currentFogColor.r = envStates[currentState]->fogColor.r;
		currentFogColor.g = envStates[currentState]->fogColor.g;
		currentFogColor.b = envStates[currentState]->fogColor.b;

		currentFogExp = envStates[currentState]->fogExp;

		currentLightColor.r = envStates[currentState]->lightColor.r;
		currentLightColor.g = envStates[currentState]->lightColor.g;
		currentLightColor.b = envStates[currentState]->lightColor.b;

		currentlightPosition.x = envStates[currentState]->lightPosition.x;
		currentlightPosition.y = envStates[currentState]->lightPosition.y;
		currentlightPosition.z = envStates[currentState]->lightPosition.z;
	}

	changeRate = envStates[currentState]->duration * 1000;

	skyColorChangeRate.r = ( envStates[nextState]->skyColor.r - envStates[currentState]->skyColor.r ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	skyColorChangeRate.g = ( envStates[nextState]->skyColor.g - envStates[currentState]->skyColor.g ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	skyColorChangeRate.b = ( envStates[nextState]->skyColor.b - envStates[currentState]->skyColor.b ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );

	fogColorChangeRate.r = ( envStates[nextState]->fogColor.r - envStates[currentState]->fogColor.r ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	fogColorChangeRate.g = ( envStates[nextState]->fogColor.g - envStates[currentState]->fogColor.g ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	fogColorChangeRate.b = ( envStates[nextState]->fogColor.b - envStates[currentState]->fogColor.b ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );

	lightColorChangeRate.r = ( envStates[nextState]->lightColor.r - envStates[currentState]->lightColor.r ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	lightColorChangeRate.g = ( envStates[nextState]->lightColor.g - envStates[currentState]->lightColor.g ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	lightColorChangeRate.b = ( envStates[nextState]->lightColor.b - envStates[currentState]->lightColor.b ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );

	lightPositionChangeRate.x = ( envStates[nextState]->lightPosition.x - envStates[currentState]->lightPosition.x ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	lightPositionChangeRate.y = ( envStates[nextState]->lightPosition.y - envStates[currentState]->lightPosition.y ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );
	lightPositionChangeRate.z = ( envStates[nextState]->lightPosition.z - envStates[currentState]->lightPosition.z ) / static_cast<float> ( ( 1000.f * envStates[currentState]->duration ) );

	fogExpRate = ( envStates[nextState]->fogExp - envStates[currentState]->fogExp ) / ( 1000.0 * envStates[currentState]->duration );

	if( skyColorChangeRate.r == 0.f && 
		skyColorChangeRate.g == 0.f &&
		skyColorChangeRate.b == 0.f &&
		fogColorChangeRate.r == 0.f &&
		fogColorChangeRate.g == 0.f &&
		fogColorChangeRate.b == 0.f &&
		lightColorChangeRate.r == 0.f &&
		lightColorChangeRate.g == 0.f &&
		lightColorChangeRate.b == 0.f &&
		lightPositionChangeRate.x == 0.f &&
		lightPositionChangeRate.y == 0.f &&
		lightPositionChangeRate.z == 0.f &&
		fogExpRate == 0.0             )
	{
		noChange = true;
	}
	else
	{
		noChange = false;
	}

	changeTimer.start();
	currentStateTimer.start();
}

void Environment::SetAutomaticStateChange( bool flag )
{
	automaticStateChange = flag;
}

void Environment::Pause()
{
	changeTimer.pause();
	currentStateTimer.pause();
}
void Environment::Unpause()
{
	changeTimer.unpause();
	currentStateTimer.unpause();
}