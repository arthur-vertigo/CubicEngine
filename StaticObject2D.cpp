#include "StaticObject2D.h"

StaticObject2D::StaticObject2D()
{
	type = "so_2d";
	hasFog = false;
	hasLighting = false;

	if( !ModelContainer::HasModel( "SYSTEM_GENERATED_BOX_MODEL" ) )
		ModelContainer::addModel( DynamicMesh::generatePlain( 1.0, 1.0, 1 ), "SYSTEM_GENERATED_BOX_MODEL" );

	this->objectModel = ModelContainer::getModel( "SYSTEM_GENERATED_BOX_MODEL" );
	this->setRotation( 90, 0.0, 0.0 );
	this->aspectRatio = false;
	this->setTint( 1.0, 1.0, 1.0, 1.0 );
}

StaticObject2D::StaticObject2D( Model* BarModel )
{
	type = "so_2d";
	hasFog = false;
	hasLighting = false;
	this->objectModel = BarModel;
	this->setRotation( 90, 0.0, 0.0 );
	this->aspectRatio = false;
	this->setTint( 1.0, 1.0, 1.0, 1.0 );
}

// Draws the object 
void StaticObject2D::draw()
{
	glFrontFace( GL_CW );
		StaticObject::draw();
	glFrontFace( GL_CCW );
}

void StaticObject2D::setSize ( double w, double h, double d )
{
	StaticObject::setSize( w, h, h );
}

void StaticObject2D::setScale( double w, double h, double d )
{
	StaticObject::setScale( w, h, h );
}

void StaticObject2D::addToScale( double w, double h, double d )
{
	StaticObject::addToScale( w, h, h );
}

void StaticObject2D::setRotation( double x, double y, double z )
{
	StaticObject::setRotation( 90, y, 0.0 );
}

void StaticObject2D::addToRotation( double x, double y, double z )
{
	StaticObject::addToRotation( 0.0, y, 0.0 );
}

void StaticObject2D::setVelocity( double x, double y, double z )
{
	StaticObject::setVelocity( x, y, 0.0 );
}

void StaticObject2D::addToVelocity( double x, double y, double z )
{
	StaticObject::addToVelocity( x, y, 0.0 );
}

void StaticObject2D::setRotationVelocity( double x, double y, double z )
{
	StaticObject::setRotationVelocity( 0.0, y, 0.0 );
}

void StaticObject2D::addToRotationVelocity( double x, double y, double z )
{
	StaticObject::addToRotationVelocity( 0.0, y, 0.0 );
}