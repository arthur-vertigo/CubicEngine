#include "DynamicMesh.h"

Model* DynamicMesh::generatePlain( double w, double d, int sections, Material* material )
{

	Model* newMesh = new Model;

	std::vector<Vertex*> verts;
	std::vector<Vertex*>::iterator verts_iter;
	std::vector<Point2D*> textures;
	std::vector<Face*> faces;
	std::vector<Bone*> bones;

	double segmentWidth = w / sections;
	double segmentDepth = d / sections;

	double halfWidth = w / 2.0;
	double halfDepth = d / 2.0;

	int xMoveDistance = sections + 1;
	int yMoveDistance = 1;

	int amountOfVerts = static_cast<int>( pow( sections+1, 2.0 ) );

	newMesh->amountOfVerts = amountOfVerts;
	newMesh->amountOfFaces = static_cast<int>(pow( sections, 2.0 ) * 2.0);
	newMesh->amountOfTextures = 4;

	newMesh->allocateMemory();

	newMesh->hasBeenAlteredSinceLoad = true;

	int vertPos = 0;

	// Create verts
	for( double i = 0; i < sections+1; i++ )
	{
		for( double j = 0; j < sections+1; j++ )
		{
			newMesh->verts[ vertPos ].position.x = i;
			newMesh->verts[ vertPos ].position.z = j;
			vertPos++;
		}
	}

	// Move verts equal distances apart, and center mesh
	for( int i = 0; i < newMesh->amountOfVerts; i++ )
	{
		newMesh->verts[ i ].position.x *= segmentWidth;
		newMesh->verts[ i ].position.x -= halfWidth;

		newMesh->verts[ i ].position.z *= segmentDepth;
		newMesh->verts[ i ].position.z -= halfDepth;
	}

	// Create the textures
	newMesh->textures[0].x = 0.0;
	newMesh->textures[0].y = 0.0;

	newMesh->textures[1].x = 0.0;
	newMesh->textures[1].y = 1.0;

	newMesh->textures[2].x = 1.0;
	newMesh->textures[2].y = 1.0;

	newMesh->textures[3].x = 1.0;
	newMesh->textures[3].y = 0.0;

	int facePos = 0;

	// Create the faces
	for( int i = 0; i < sections; i++ )
	{
		for( int j = 0; j < sections; j++ )
		{
			newMesh->faces[facePos].verts[0] = &newMesh->verts[( i * xMoveDistance ) + ( j * yMoveDistance )];
			newMesh->faces[facePos].verts[1] = &newMesh->verts[( i * xMoveDistance ) + ( (j+1) * yMoveDistance )];
			newMesh->faces[facePos].verts[2] = &newMesh->verts[( (i+1) * xMoveDistance ) + ( (j+1) * yMoveDistance )];
			newMesh->faces[facePos].textures[0] = &newMesh->textures[0];
			newMesh->faces[facePos].textures[1] = &newMesh->textures[1];
			newMesh->faces[facePos].textures[2] = &newMesh->textures[2];
			if( material != NULL )
				newMesh->faces[facePos].material = material->name;
			facePos++;

			newMesh->faces[facePos].verts[0] = &newMesh->verts[( i * xMoveDistance ) + ( j * yMoveDistance )];
			newMesh->faces[facePos].verts[1] = &newMesh->verts[( (i+1) * xMoveDistance ) + ( (j+1) * yMoveDistance )];
			newMesh->faces[facePos].verts[2] = &newMesh->verts[( (i+1) * xMoveDistance ) + ( j * yMoveDistance )];
			newMesh->faces[facePos].textures[0] = &newMesh->textures[0];
			newMesh->faces[facePos].textures[1] = &newMesh->textures[2];
			newMesh->faces[facePos].textures[2] = &newMesh->textures[3];
			if( material != NULL )
				newMesh->faces[facePos].material = material->name;
			facePos++;

		}
	}

	newMesh->calcSizeAndCenter();
	newMesh->calcNormals();
	newMesh->setupRenderPackages();
	newMesh->updateRenderPackages();
	newMesh->loaded = true;

	return newMesh;
}

Model* DynamicMesh::generateBox( double w, double h, double d, int sections, std::string material )
{
	return NULL;
}

void DynamicMesh::addNoise( double x, double y, double z, Model* model )
{

	if( model == NULL )
		return;
	
	std::vector<Vertex*>::iterator iter;
	
	for( int i = 0; i < model->amountOfVerts+1; i++ )
	{
		
		model->verts[i].position.x += Math::randomDouble( 0.0, x );
		model->verts[i].position.y += Math::randomDouble( 0.0, y );
		model->verts[i].position.z += Math::randomDouble( 0.0, z );
	}

	model->calcNormals();
	model->calcSizeAndCenter();
	model->updateRenderPackages();
}

// Will multiply the modes pixels by a sin wave
void DynamicMesh::addSin( Point3D position, double size,  double a, double per, double phs, StaticObject* obj, bool (*DynamicMeshFunction)(Model*) )
{

	Model* model = obj->getModel();
	Point3D objPos = obj->GetPosition();
	bool changedVerts = false;

	if( model == NULL )
		return;

	std::vector<Vertex*>::iterator iter;

	for( int i = 0; i < model->amountOfVerts+1; i++ )
	{

		double distanceFromPos = sqrt( pow( position.x - ( model->verts[i].position.x + objPos.x ), 2.0 ) + pow( position.z - ( model->verts[i].position.z + objPos.z ), 2.0 ) );

		if( distanceFromPos > size ) 
			continue;

		model->verts[i].position.y = ( a * sin( per * distanceFromPos + phs ) );
		changedVerts = true;
	}

	if( changedVerts )
	{

		if( DynamicMeshFunction != NULL )
		{
			if( DynamicMeshFunction( model ) )
				model->setupRenderPackages();
		}

		model->hasBeenAlteredSinceLoad = true;
		model->calcNormals();
		model->updateRenderPackages();
	}
}

void DynamicMesh::addPress( Point3D position, double pressure, double size, double falloff, double damper, StaticObject* obj, bool (*DynamicMeshFunction)(Model*) )
{

	if( obj != NULL && obj->getModel() == NULL )
		return;

	Model* model = obj->getModel();
	Point3D objPos = obj->GetPosition();
	bool changedVerts = false;

	for( int i = 0; i < model->amountOfVerts+1; i++ )
	{
		double distanceFromPos = sqrt( pow( position.x - ( model->verts[i].position.x + objPos.x ), 2.0 ) + pow( position.z - ( model->verts[i].position.z + objPos.z ), 2.0 ) );

		if( distanceFromPos > size || ( pressure - ( distanceFromPos / falloff ) ) < 0 ) 
			continue;

		model->verts[i].position.y -= ( ( pressure - ( distanceFromPos / falloff ) ) * damper );
		changedVerts = true;
	}

	if( changedVerts )
	{

		if( DynamicMeshFunction != NULL )
		{
			if( DynamicMeshFunction( model ) )
				model->setupRenderPackages();
		}
			model->hasBeenAlteredSinceLoad = true;
			model->calcNormals();
			model->updateRenderPackages();
	}
}

void DynamicMesh::Flatten( Point3D position, double size, double zero, double rate, StaticObject* obj, bool (*DynamicMeshFunction)(Model*) )
{

	if( obj != NULL && obj->getModel() == NULL )
		return;

	Model* model = obj->getModel();
	Point3D objPos = obj->GetPosition();
	bool changedVerts = false;

	for( int i = 0; i < model->amountOfVerts+1; i++ )
	{
		double distanceFromPos = sqrt( pow( position.x - ( model->verts[i].position.x + objPos.x ), 2.0 ) + pow( position.z - ( model->verts[i].position.z + objPos.z ), 2.0 ) );

		if( distanceFromPos > size ) 
			continue;

		if( model->verts[i].position.y < zero )
		{
			model->verts[i].position.y += rate;

			if( model->verts[i].position.y > zero )
			{
				model->verts[i].position.y = zero;
			}
		}
		else if ( model->verts[i].position.y > zero )
		{
			model->verts[i].position.y -= rate;

			if( model->verts[i].position.y < zero )
			{
				model->verts[i].position.y = zero;
			}
		}

		changedVerts = true;
	}

	if( changedVerts )
	{

		if( DynamicMeshFunction != NULL )
		{
			if( DynamicMeshFunction( model ) )
				model->setupRenderPackages();
		}
			model->hasBeenAlteredSinceLoad = true;
			model->calcNormals();
			model->updateRenderPackages();
	}

}