#include "StaticObject.h"

StaticObject::StaticObject()
{
	type = "so";

	scale.w = 1.0;
	scale.h = 1.0;
	scale.d = 1.0;

	size.w = 1.0;
	size.h = 1.0;
	size.d = 1.0;

	tint.r = 0.0;
	tint.g = 0.0;
	tint.b = 0.0;
	tint.a = 1.0;

	overrideTexture = -1;

	objectModel = NULL;
	garbageCollect = false;

	aspectRatio = true;
	smoothShade = true;
}

bool StaticObject::setModel( Model* newModel )
{
	if( newModel != NULL && newModel->hasLoaded() )
	{
		objectModel = newModel;
		size = newModel->getSize();
		return true;
	}

	return false;
}

Model* StaticObject::getModel()
{
	return objectModel;
}

void StaticObject::SetTexture( GLuint newTexture )
{
	overrideTexture = newTexture;
}

bool StaticObject::PrepareDraw()
{
	if( !visible || tint.a <= 0.0 )
	{
		return false;
	}

	if( smoothShade )
	{
		glShadeModel( GL_SMOOTH );
	}
	else
	{
		glShadeModel( GL_FLAT );
	}

	if( hasFog )
	{
		if( !glIsEnabled( GL_FOG ) ) 
			glEnable( GL_FOG ); 
	}
	else
	{
		if( glIsEnabled( GL_FOG ) ) 
			glDisable( GL_FOG ); 
	}

	if( hasLighting )
	{
		if( !glIsEnabled( GL_LIGHTING ) ) 
			glEnable( GL_LIGHTING ); 
	}
	else
	{
		if( glIsEnabled( GL_LIGHTING ) ) 
			glDisable( GL_LIGHTING ); 
	}

	return true;
}

void StaticObject::draw()
{
	if( !PrepareDraw() )
		return;

	if( objectModel != NULL )
		objectModel->draw( position, scale, rotation, tint, overrideTexture );
}

void StaticObject::drawDebugInfo( double length )
{
	if( objectModel != NULL )
	{
		objectModel->drawNormals( position, scale, rotation, length );
		//objectModel->drawPoints( position, rotation, 1.0 );
	}
}

void StaticObject::move()
{
	position.x += ( moveVelocity.x * moveVelocity.mag ) * Math::timeFactor( moveTimer.get_ticks() );
	position.y += ( moveVelocity.y * moveVelocity.mag ) * Math::timeFactor( moveTimer.get_ticks() );
	position.z += ( moveVelocity.z * moveVelocity.mag ) * Math::timeFactor( moveTimer.get_ticks() );

	rotation.x += ( rotateVelocity.x * rotateVelocity.mag ) * Math::timeFactor( moveTimer.get_ticks() );
	rotation.y += ( rotateVelocity.y * rotateVelocity.mag ) * Math::timeFactor( moveTimer.get_ticks() );
	rotation.z += ( rotateVelocity.z * rotateVelocity.mag ) * Math::timeFactor( moveTimer.get_ticks() );

	moveTimer.start();
	rotation.keepInRange();
}

// Metric mutators
void StaticObject::setPosition( double x, double y, double z )
{
	position.x = x;
	position.y = y;
	position.z = z;
}

// Metric mutators
void StaticObject::setPosition( Point3D position )
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}

void StaticObject::addToPosition( double x, double y, double z )
{
	position.x += x;
	position.y += y;
	position.z += z;
}

Point3D StaticObject::GetPosition()
{
	return position;
}

void StaticObject::setSmoothShading( bool newShading )
{
	smoothShade = newShading;
}

// Size and scale
void StaticObject::setMaintainAspectRatio( bool flag )
{
	aspectRatio = flag;
}

void StaticObject::setSize ( double w, double h, double d )
{
	double xMult = w / size.w;
	double yMult = h / size.h;
	double zMult = d / size.d;

	if( !aspectRatio )
	{
		setScale( xMult, yMult, zMult );
		return;
	}
	
	double areaOfXScale = (size.w * xMult) + (size.h * xMult) + (size.d * xMult);
	double areaOfYScale = (size.w * yMult) + (size.h * yMult) + (size.d * yMult);
	double areaOfZScale = (size.w * zMult) + (size.h * zMult) + (size.d * zMult);


	if( (size.w * yMult) > h || (size.d * yMult) > h )
	{
		yMult = 0;
	}

	if( (size.h * xMult) > w || (size.d * xMult)  > w )
	{
		xMult = 0;
	}

	if( (size.w * zMult) > d || (size.h * zMult)  > d )
	{
		zMult = 0;
	}

	double largestValue = 0.0;
	double* useValue = NULL;

	if( areaOfYScale > largestValue && yMult != 0 )
	{
		largestValue = areaOfXScale;
		useValue = &yMult;
	}

	if( areaOfXScale > largestValue && xMult != 0 )
	{
		largestValue = areaOfXScale;
		useValue = &xMult;
	}

	if( areaOfZScale > largestValue && zMult != 0 )
	{
		useValue = &zMult;
	}

	if( !useValue == NULL )
	{
		setScale( *useValue, *useValue, *useValue );
	}
}

void StaticObject::setScale( double w, double h, double d )
{
	if( w > 0 )
		scale.w = w;

	if( h > 0 )
		scale.h = h;

	if( d > 0 )
		scale.d = d;

	worldSize.w = size.w * scale.w;
	worldSize.h = size.h * scale.w;
	worldSize.d = size.d * scale.w;
}

void StaticObject::addToScale( double w, double h, double d )
{
	scale.w += w;
	scale.h += h;
	scale.d += d;
}

Size3D StaticObject::GetSize()
{
	return worldSize;
}

void StaticObject::setRotation( double x, double y, double z )
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void StaticObject::setRotation( Rotation3D rotation )
{
	this->rotation.x = rotation.x;
	this->rotation.y = rotation.y;
	this->rotation.z = rotation.z;
}

void StaticObject::addToRotation( double x, double y, double z )
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;

	rotation.keepInRange();
}

void StaticObject::setVelocity( double x, double y, double z )
{
	moveVelocity.x = x;
	moveVelocity.y = y;
	moveVelocity.z = z;
}

void StaticObject::addToVelocity( double x, double y, double z )
{
	moveVelocity.x += x;
	moveVelocity.y += y;
	moveVelocity.z += z;
}

void StaticObject::setRotationVelocity( double x, double y, double z )
{
	rotateVelocity.x = x;
	rotateVelocity.y = y;
	rotateVelocity.z = z;
}

void StaticObject::addToRotationVelocity( double x, double y, double z )
{
	rotateVelocity.x += x;
	rotateVelocity.y += y;
	rotateVelocity.z += z;
}

void StaticObject::setTint( float r, float g, float b, float a )
{
	tint.r = r;
	tint.g = g;
	tint.b = b;
	tint.a = a;
}

void StaticObject::Pause()
{
	moveTimer.pause();
	eventTimer.pause();
}

void StaticObject::Unpause()
{
	moveTimer.unpause();
	eventTimer.unpause();
}