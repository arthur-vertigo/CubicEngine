#include "Camera.h"

Camera* Camera::currentCamera = NULL;

Camera::Camera()
{
	up.y = 1.0;
}

void Camera::resetCamera()
{
	currentCamera = NULL;
	//glPopMatrix();
}

void Camera::draw()
{
	Size3D temp( 1.0, 1.0, 1.0 );
	GLDebug::drawPoint3D( position, temp, 1.0, GLDebug::SQUARE );
}

void Camera::setView()
{
	if( currentCamera != this )
	{
		currentCamera = this;
		//glPushMatrix();
	}

	gluLookAt ( position.x, position.y, position.z, position.x+(sin( Math::DegToRad( rotation.y ) )), position.y+ rotation.x, position.z-(cos( Math::DegToRad( rotation.y )  )), up.x, up.y, up.z);
}

void Camera::addToPosition( double x, double y, double z )
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Camera::setPosition( double x, double y, double z )
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::setPosition( Point3D position )
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}


Point3D Camera::getPosition()
{
	return position;
}

void Camera::setRotation( double x, double y, double z )
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	rotation.keepInRange();
}

void Camera::setRotation( Rotation3D rotation )
{
	this->rotation.x = rotation.x;
	this->rotation.y = rotation.y;
	this->rotation.z = rotation.z;

	this->rotation.keepInRange();
}

void Camera::addToRotation( double x, double y, double z )
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;

	rotation.keepInRange();
}

Rotation3D Camera::getRotation()
{
	return rotation;
}