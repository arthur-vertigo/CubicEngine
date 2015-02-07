#define GL_GLEXT_PROTOTYPES

/***

Model: Defines a collection of faces, vertexs, and texture coordiants to form a 3d model

***/

#ifndef MODEL_H
#define MODEL_H

#include "Metrics.h"
#include "Face.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
#include "StringFunctions.h"
#include "Math.h"
#include "GL_Debug.h"
#include "GLTextureContainer.h"
#include "MaterialContainer.h"
#include "Utilities.h"
#include "OctTree.h"
#include <string>
#include <vector>
#include <fstream>

// The bone class container any bone information about the model
class Bone
{
public:
	std::string name;
	std::vector<Vertex*> effectedVerts;
private:
};

// Contains a per-vertex package of information for vertex arrays
//  -Used to provide faster rendering
class RenderPacket
{
public:
	Point3F position;
	Vector3F normal;
	Point2F texture;
	Color4F color;
	GLubyte padding[12];
	RenderPacket( );
	RenderPacket( Point3F position, Vector3F normal, Point2F texture, Color4F color );
};

class Model
{
private:

	// Vbos and Ibos
	std::map<std::string, GLuint> VBOs;
	std::map<std::string, GLuint> IBOs;

	// Map of vertex positions in their array
	std::map<Vertex*,GLuint> vertexMap;

	// Indices for vertex arrays
	GLuint* indices;

	// Packages of information for rendering
	bool renderPackagesSetup;
	std::map<std::string, RenderPacket*> RenderPackages;
	std::map<std::string, int> RenderPackagesSize;
	std::map<std::string, GLuint*> RenderPackagesIndices;

	// Material
	Material* material;

	// Texture coordinates
	Point2D* textures;
	int amountOfTextures;

	// Bones related to the model
	Bone* bones;
	int amountOfBones;

	// True is the model has been suscessfully loaded
	bool loaded;

	// Size of the model in world coordinates
	Size3D size;

	// Will calculate the size of the model and center the verts
	void calcSizeAndCenter( bool center = true );

	// Alternative size calculation function
	void CalcSize();

	// Allocates memory for the render packages
	void setupRenderPackages();

	// Name of the model
	std::string name;

	// Allocated memory for the model
	void allocateMemory();

	// True if the memory for the model has been allocated
	bool memoryAllocated;

	// True if the model has been changed since load time
	bool hasBeenAlteredSinceLoad;

public:
	// Friend class to allow DynamicMesh to alter the verts
	friend class DynamicMesh;

	// Faces in the model
	Face* faces;
	int amountOfFaces;

	// Vertex positions
	int amountOfVerts;
	Vertex* verts;

	Model();
	~Model();
	void ClearVBO_IBOs();

	// Updates the render packages
	void updateRenderPackages();

	// Loads a .obj file at path location
	// -allowIncompleteLoad will allow a model to load without having the associated texture files
	bool load( std::string path, bool allowIncompleteLoad = false, bool sizeAndCenter = true );

	// Loads a model with supplied vertex, texture, face, and bone data
	// -allowIncompleteLoad will allow a model to load without having the associated texture files
	bool load( std::vector<Vertex*>& verts, std::vector<Point2D*>& textures, std::vector<Face*>& faces, std::vector<Bone*>& bones, bool allowIncompleteLoad = false );

	// Saves a .obj file to path
	bool save( std::string path );

	void exportRenderPackets( std::string path );

	// Unloads the models data
	void unload();

	// Returns true if the model has loaded correctly
	bool hasLoaded();

	// Draws the model at the specified position, scale, and rotation *refactor*
	void draw( Point3D position, Size3D scale, Rotation3D rotation, Color4F color, GLuint overrideTexture = 0 );

	// Calculates the vertex normals for the entire model
	void calcNormals( bool force = false);

	// Draws the normal vector for each vertex 
	void drawNormals( Point3D position, Size3D scale, Rotation3D rotation, double length );

	// Draws the position of each vertex
	void drawPoints( Point3D position, Size3D scale, Rotation3D rotation, double length );

	// Returns the size of the model
	Size3D getSize();

	// Gets / Sets the name of the model
	std::string Name();
	void Name( std::string name );
};

#endif // !MODEL_H
