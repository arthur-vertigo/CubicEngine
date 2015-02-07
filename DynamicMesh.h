/***

Dynamic Mesh: Allows generation of dynamic meshs. 
               -These meshes are in full complience with the 
			    Model class and can be exported and imported
				as .obj files

***/

#ifndef DYNAMIC_MESH_H
#define DYNAMIC_MESH_H

#include "Metrics.h"
#include "Model.h"
#include "Material.h"
#include "MaterialContainer.h"
#include "StaticObject.h"
#include <vector>

class DynamicMesh
{
public:

	// Object Creation

	// Will create a W x D plain with the specified sections and material
	static Model* generatePlain( double w, double d, int sections, Material* material = NULL );

	// Will create a W x H x D box *not finished*
	static Model* generateBox( double w, double h, double d, int sections, std::string material = "" );

	// Object Manipulation

	// Will cause noise in the supplied model
	static void addNoise( double x, double y, double z, Model* model );

	// Will multiply the models y coord by a sin wave
	static void addSin( Point3D position, double size,  double a, double per, double phs, StaticObject* obj, bool (*DynamicMeshFunction)(Model*) = NULL );

	// Presses on the mesh
	static void addPress( Point3D position, double pressure, double size, double falloff, double damper, StaticObject* obj, bool (*DynamicMeshFunction)(Model*) = NULL );

	// Flattens the mesh
	static void Flatten( Point3D position, double size, double zero, double rate, StaticObject* obj, bool (*DynamicMeshFunction)(Model*) = NULL );
};

#endif // !DYNAMIC_MESH_H
