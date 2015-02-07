#define GL_GLEXT_PROTOTYPES
#include "Model.h"

// Buffer offset macro for VBO
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Function pointers for opengl 
typedef void (APIENTRY * GL_GenBuffers_Func)( GLsizei, GLuint* );
GL_GenBuffers_Func glGenBuffers_ptr = 0;

typedef void (APIENTRY * GL_DeleteBuffers_Func)( GLsizei, GLuint* );
GL_DeleteBuffers_Func glDeleteBuffers_ptr = 0;

typedef void (APIENTRY * GL_BindBuffer_Func)( GLenum, GLuint );
GL_BindBuffer_Func glBindBuffer_ptr = 0;

typedef void (APIENTRY * GL_BufferData_Func)( GLsizei, GLsizeiptr, const GLvoid *, GLenum );
GL_BufferData_Func glBufferData_ptr = 0;

typedef void (APIENTRY * GL_BufferSubData_Func)( GLenum , GLintptr, GLsizeiptr, const GLvoid * );
GL_BufferSubData_Func glBufferSubData_ptr = 0;

const char ITEM_DELIMITER = ' ';
const char PRAM_DELIMITER = '/';

const int VBO_IBO = 1;

RenderPacket::RenderPacket( )
{
}

RenderPacket::RenderPacket( Point3F position, Vector3F normal, Point2F texture, Color4F color )
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;

	this->normal.x = normal.x;
	this->normal.y = normal.y;
	this->normal.z = normal.z;

	this->texture.x = texture.x;
	this->texture.y = texture.y;

	this->color.r = color.r;
	this->color.g = color.g;
	this->color.b = color.b;
	this->color.a = color.a;
}

////////////////////
///// Model Class //
////////////////////

Model::Model()
{

	indices = NULL;
	memoryAllocated = false;
	amountOfVerts = 0;
	amountOfFaces = 0;
	amountOfTextures = 0;
	amountOfBones = 0;
	loaded = false;
	hasBeenAlteredSinceLoad = false;

	// Setup the opengl function pointers
	if( glGenBuffers_ptr == NULL )
	{
		glGenBuffers_ptr = (GL_GenBuffers_Func) SDL_GL_GetProcAddress("glGenBuffers");
		glDeleteBuffers_ptr = (GL_GenBuffers_Func) SDL_GL_GetProcAddress("glDeleteBuffers");
		glBindBuffer_ptr = (GL_BindBuffer_Func) SDL_GL_GetProcAddress("glBindBuffer");
		glBufferData_ptr = (GL_BufferData_Func) SDL_GL_GetProcAddress("glBufferData");
		glBufferSubData_ptr = (GL_BufferSubData_Func) SDL_GL_GetProcAddress("glBufferSubData");
	}

}

Model::~Model()
{
	if( memoryAllocated )
	{
		delete []this->verts;
		delete []this->faces;
		delete []this->textures;
		delete []this->bones;
		delete []this->indices;
		vertexMap.clear();
	}

	if( renderPackagesSetup )
	{
		std::map<std::string,int>::iterator iter;
		std::string texture;
		
		for( iter = RenderPackagesSize.begin(); iter != RenderPackagesSize.end(); iter++ )
		{
			// Delete render packages
			delete[] RenderPackagesIndices[ (iter)->first ];
			delete[] RenderPackages[ (iter)->first ];

			// Delete gpu buffers
			glDeleteBuffers_ptr( 1, &VBOs[ (iter)->first ] );
			glDeleteBuffers_ptr( 1, &IBOs[ (iter)->first ] );
		}
		
		// Clear the non controlled dynamic members
		RenderPackagesSize.clear();
		RenderPackagesIndices.clear();
		RenderPackages.clear();
		VBOs.clear();
		IBOs.clear();

	}

}

void Model::ClearVBO_IBOs()
{
	if( renderPackagesSetup )
	{
		std::map<std::string,int>::iterator iter;
		std::string texture;
		
		for( iter = RenderPackagesSize.begin(); iter != RenderPackagesSize.end(); iter++ )
		{
			// Delete render packages
			delete[] RenderPackagesIndices[ (iter)->first ];
			delete[] RenderPackages[ (iter)->first ];

			// Delete gpu buffers
			glDeleteBuffers_ptr( 1, &VBOs[ (iter)->first ] );
			glDeleteBuffers_ptr( 1, &IBOs[ (iter)->first ] );
		}
		
		// Clear the non controlled dynamic members
		RenderPackagesSize.clear();
		RenderPackagesIndices.clear();
		RenderPackages.clear();
		VBOs.clear();
		IBOs.clear();

	}

	loaded = false;
}

bool Model::hasLoaded()
{
	return loaded;
}

void Model::unload()
{
}

bool Model::load( std::vector<Vertex*>& verts, std::vector<Point2D*>& textures, std::vector<Face*>& faces, std::vector<Bone*>& bones, bool allowIncompleteLoad )
{
	return true;
}

void Model::allocateMemory()
{
	// Delete memory if already allocated
	if( memoryAllocated )
	{
		delete []this->verts;
		delete []this->faces;
		delete []this->textures;
		delete []this->bones;
		delete []this->indices;
		vertexMap.clear();
	}

	// Allocate memory
	verts = new Vertex[ amountOfVerts ] ;
	faces = new Face[ amountOfFaces+10 ] ;
	textures = new Point2D[ amountOfTextures+10 ] ;
	bones = new Bone[ amountOfBones+10 ] ;
	indices = new GLuint[ amountOfFaces * 3 ];

	// setup indices
	for( int i = 0; i < amountOfFaces * 3; i++ )
	{
		indices[i] = i;
	}

	// Map verts to ints
	for( int i = 0; i < amountOfVerts; i++ )
	{
		vertexMap[ &verts[i] ] = i;
	}

	memoryAllocated = true;
}

bool Model::load( std::string path, bool allowIncompleteLoad,  bool sizeAndCenter )
{

	std::string line;
	std::ifstream myfile ( path );

	// Get the root path string for material loading
	StringFunctions::EraseUntilCharFound( path, false, '/' );
	std::string root_path = path;

	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	double nX = 0.0;
	double nY = 0.0;
	double nZ = 0.0;

	double tX = 0.0;
	double tY = 0.0;

	int numOfPrams = 0;
	int numOfItems = 0;

	int vertPosition = 0;
	int normalPosition = 0;
	int texturePos[3] = {0};

	Material* currentMaterial = NULL;
	std::string currentMaterialName = "";

	int vertPos = 1;
	int textPos = 1;
	int facePos = 0;
	int bonePos = 0;

	amountOfVerts = 0;
	amountOfFaces = 0;
	amountOfTextures = 0;
	amountOfBones = 0;

	// Get number of verts in the file
	if (myfile.is_open())
	{
		amountOfVerts++;
		while ( myfile.good() )
		{
			getline ( myfile, line );

			// Ignore comments
			if( line[0] == '#' )
				continue;

			// Verts
			if( line[0] == 'v' && line[1] == ' ' )
			{
				amountOfVerts++;
				continue;
			}

			// Textures
			if( line[0] == 'v' && line[1] == 't' )
			{
				amountOfTextures++;
				continue;
			}

			// Faces
			if( line[0] == 'f' && line[1] == ' ' )
			{
				amountOfFaces++;
				continue;
			}
		}

		myfile.clear();
		myfile.seekg( 0, std::ios::beg );
	}
	else
	{
		return false;
	}

	// Allocate memory after finding out how much is needed
	// this could be made more efficent
	allocateMemory();

	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline ( myfile, line );

			// Remove multi spaces
			StringFunctions::spliceOut( line, "  ", " " );

			// Ignore comments
			if( line[0] == '#' )
			{
				continue;
			}

			// Load materials from material files
			if( StringFunctions::beginsWith( line, "mtllib" ) )
			{
				if( !MaterialContainer::loadMaterialFile( root_path + StringFunctions::getPram( line, 2, ' ' ) ) && !allowIncompleteLoad )
				{
					return false;
				}
				else
				{
					continue;
				}
			}

			// Material Flag
			if( StringFunctions::beginsWith( line, "usemtl" ) )
			{
				if( MaterialContainer::hasMaterial( StringFunctions::getPram( line, 2, ' ' ) ) )
				{
					currentMaterial = MaterialContainer::getMaterial( StringFunctions::getPram( line, 2, ' ' ) );
				}
				else
				{
					continue;
				}

			}

			// Textures
			if( StringFunctions::beginsWith( line, "vt" ) )
			{
				tX = 0.0;
				tY = 0.0;

				numOfPrams = StringFunctions::getNumOfPrams( StringFunctions::getPram( line, 1, ITEM_DELIMITER ), PRAM_DELIMITER );

				if( numOfPrams == 1 )
				{
					tX = atof( StringFunctions::getPram( line, 2, ITEM_DELIMITER).c_str() );
					tY = atof( StringFunctions::getPram( line, 3, ITEM_DELIMITER).c_str() );
				}
				else
				{
					continue;
				}

				textures[textPos].x = tX;
				textures[textPos].y = tY;
				textPos++;
				continue;
			}

			// Ignore normals
			if( StringFunctions::beginsWith( line, "vn" ) )
			{
				continue;
			}

			// Vertexs
			if( StringFunctions::beginsWith( line, "v" ) )
			{
				nX = 0.0;
				nY = 0.0;
				nZ = 0.0;
				tX = 0.0;
				tY = 0.0;

				x = atof( StringFunctions::getPram( line, 2, ITEM_DELIMITER).c_str() );
				y = atof( StringFunctions::getPram( line, 3, ITEM_DELIMITER).c_str() );
				z = atof( StringFunctions::getPram( line, 4, ITEM_DELIMITER).c_str() );

				verts[vertPos].position.x = x;
				verts[vertPos].position.y = y;
				verts[vertPos].position.z = z;
				vertPos++;

				continue;
			}

			// Faces
			if( line[0] == 'f' )
			{

				numOfPrams = StringFunctions::getNumOfPrams( StringFunctions::getPram( line, 2, ITEM_DELIMITER ), PRAM_DELIMITER );
				numOfItems = StringFunctions::getNumOfPrams( line, ITEM_DELIMITER )-1;

				switch( numOfPrams )
				{
				case 3:
					case 2:
						texturePos[0] = atoi(StringFunctions::getPram( StringFunctions::getPram( line, 2, ITEM_DELIMITER), 2, PRAM_DELIMITER ).c_str() );
						texturePos[1] = atoi(StringFunctions::getPram( StringFunctions::getPram( line, 3, ITEM_DELIMITER), 2, PRAM_DELIMITER ).c_str() );
						texturePos[2] = atoi(StringFunctions::getPram( StringFunctions::getPram( line, 4, ITEM_DELIMITER), 2, PRAM_DELIMITER ).c_str() );

						if( numOfItems >= 4 )
						{
						}
					case 1:
					default:

						faces[facePos].verts[0] =  &verts[ atoi(StringFunctions::getPram( line, 2, ITEM_DELIMITER).c_str())];
						faces[facePos].verts[1] =  &verts[ atoi(StringFunctions::getPram( line, 3, ITEM_DELIMITER).c_str())];
						faces[facePos].verts[2] =  &verts[ atoi(StringFunctions::getPram( line, 4, ITEM_DELIMITER).c_str())];
						// needed for when quads are implimented
						//newFace->verts[3] = verts[ atof(StringFunctions::getPram( line, 5, ITEM_DELIMITER).c_str())];

						if( currentMaterial != NULL )
							faces[facePos].material =  currentMaterial->name;

						faces[facePos].textures[0] = &textures[texturePos[0]];
						faces[facePos].textures[1] = &textures[texturePos[1]];
						faces[facePos].textures[2] = &textures[texturePos[2]];
						// needed for when quads are implimented
						//newFace->textures[3] = textures[texturePos[3]];
						facePos++;

					break;
				}


				continue;
			}
		}

		if( sizeAndCenter )
		{
			calcSizeAndCenter();
		}
		else
		{
			CalcSize();
		}

		calcNormals();
		setupRenderPackages();
		updateRenderPackages();
	}
	else
	{ // False path for model loading
		loaded = false;
		return false;
	}

	loaded = true;
	return true;
}

void Model::CalcSize()
{
	calcSizeAndCenter( false );
}

void Model::calcSizeAndCenter( bool center )
{
	double xMin = 0.0;
	double xMax = 0.0;

	double yMin = 0.0;
	double yMax = 0.0;

	double zMin = 0.0;
	double zMax = 0.0;

	for( int i = 0; i < amountOfVerts; i++ )
	{
		if( verts[i].position.x < xMin )
			xMin = verts[i].position.x;

		if( verts[i].position.x > xMax )
			xMax = verts[i].position.x;

		if( verts[i].position.y < yMin )
			yMin = verts[i].position.y;

		if( verts[i].position.y > yMax )
			yMax = verts[i].position.y;

		if( verts[i].position.z < zMin )
			zMin = verts[i].position.z;

		if( verts[i].position.z > zMax )
			zMax = verts[i].position.z;
	}

	size.w = xMax - xMin;
	size.h = yMax - yMin;
	size.d = zMax - zMin;

	if( center )
	{
		double xOffset = xMin + ( size.w / 2.0 );
		double yOffset = yMin + ( size.h / 2.0 );
		double zOffset = zMin + ( size.d / 2.0 );

		// Move all verts to center the model
		for( int i = 0; i < amountOfVerts; i++ )
		{
			verts[i].position.x -= xOffset;
			verts[i].position.y -= yOffset;
			verts[i].position.z -= zOffset;
		}
	}
}

bool Model::save( std::string path )
{
	if( !loaded )
		return false;

	if( !hasBeenAlteredSinceLoad )
		return false;

	std::map< Point2D*, int > textureMap;
	std::map< Vertex*, int > vertMap2;
	int vert = 1;
	int text = 1;

	std::string currentMaterial = "";

	std::string newLine = "\n";

	std::ofstream modelFile;
	modelFile.open ( path );
	modelFile << "#Created by Cubic Engine, ©AW-Games.com 2012-2013, All Rights Reserved" << newLine;

	// Output verts
	vertexMap.clear();
	modelFile << "#Vertexs" << newLine;
	for( int i = 0; i < amountOfVerts; i++ )
	{
		modelFile << "v " << verts[i].position.x << " " << verts[i].position.y << " " << verts[i].position.z << newLine;
		vertexMap[ &verts[i] ] = vert++;
	}

	// Output texture coords
	modelFile << "#Textures" << newLine;
	for( int i = 1; i < amountOfTextures+1; i++ )
	{
		modelFile << "vt " << textures[i].x << " " << textures[i].y << newLine;
		textureMap[ &textures[i] ] = text;
		text++;
	}

	// Output faces
	modelFile << "#Faces" << newLine;
	for( int i = 0; i < amountOfFaces; i++ )
	{
		// Output change for diffrent face materials
		if( faces[i].material != currentMaterial )
		{
			currentMaterial = faces[i].material;
			modelFile << "usemtl " << currentMaterial << newLine;
		}

		modelFile << "f " <<	vertexMap[faces[i].verts[0]] << "/" << textureMap[faces[i].textures[0]] << "/ " <<
								vertexMap[faces[i].verts[1]] << "/" << textureMap[faces[i].textures[1]] << "/ " <<
								vertexMap[faces[i].verts[2]] << "/" << textureMap[faces[i].textures[2]] << "/ " << newLine;
	}

	modelFile.close();
		
	return true;
}

std::string Model::Name()
{
	return name;
}

void Model::Name( std::string name )
{
	this->name = name;
}

void Model::exportRenderPackets( std::string path )
{
	std::ofstream rpFile;
	rpFile.open ( path );


	std::map<std::string, RenderPacket*>::iterator iter;
	RenderPacket* currentPacket = RenderPackages.begin()->second;

	for( int i = 0; i < amountOfFaces*3; i++ )
	{
		rpFile << "{\n";
		rpFile << "\tVert: " << currentPacket[i].position.x << " " << currentPacket[i].position.y << " " << currentPacket[i].position.z << " " << "\n";
		rpFile << "\tColor: " << currentPacket[i].color.r << " " << currentPacket[i].color.g << " " << currentPacket[i].color.b << " " << currentPacket[i].color.a << " " << "\n";
		rpFile << "\tNormal: " << currentPacket[i].normal.x << " " << currentPacket[i].normal.y << " " << currentPacket[i].normal.z << " " << "\n";
		rpFile << "\tTexture: " << currentPacket[i].texture.x << " " << currentPacket[i].texture.y << "\n";
		rpFile << "}\n";
	}

	rpFile.close();
}

void Model::setupRenderPackages()
{
	// Cleanup the old render packages if available
	if( renderPackagesSetup )
	{
		for( auto iter = RenderPackagesSize.begin(); iter != RenderPackagesSize.end(); iter++ )
		{
			// Delete render packages
			delete[] RenderPackagesIndices[ (iter)->first ];
			delete[] RenderPackages[ (iter)->first ];

			// Delete gpu buffers
			glDeleteBuffers_ptr( 1, &VBOs[ (iter)->first ] );
			glDeleteBuffers_ptr( 1, &IBOs[ (iter)->first ] );
		}
		
		// Clear the non dynamic containers
		RenderPackagesSize.clear();
		RenderPackagesIndices.clear();
		RenderPackages.clear();
		VBOs.clear();
		IBOs.clear();

	}

	std::string material;
	int size;

	// Count the amount of each package needs to be made
	for( int i = 0; i < amountOfFaces; i++ )
		RenderPackagesSize[ faces[i].material ]++;

	for( auto iter = RenderPackagesSize.begin(); iter != RenderPackagesSize.end(); iter++ )
	{
		material = (iter)->first;
		size = (iter)->second ;
		RenderPackagesIndices[ material ] = new GLuint[ size * 3 ];
		RenderPackages[ material ] = new RenderPacket[ size * 3 ];

		// Create the indice list for each material
		for( int i = 0; i < size * 3; i++ )
			RenderPackagesIndices[ material ][i] = i;
	
		if( VBO_IBO )
		{
			// Generate a GLuint for both the vertex buffer and a indice buffer
			glGenBuffers_ptr( 1, &VBOs[ material ] );
			glGenBuffers_ptr( 1, &IBOs[ material ] );
		}
	}

	renderPackagesSetup = true;
}

void Model::updateRenderPackages()
{

	std::map<std::string,int> useageMap;
	int currentPos = 0;
	RenderPacket* currentPacketType = NULL;

	for( int i = 0; i < amountOfFaces; i++ )
	{
		// Current position for material type
		currentPos = useageMap[ faces[i].material ];
		useageMap[ faces[i].material ] += 3;

		// The materials package
		currentPacketType = RenderPackages[ faces[i].material ];

		// The faces of the triangle
		currentPacketType[currentPos].color.r = faces[i].verts[0]->color.r;
		currentPacketType[currentPos].color.g = faces[i].verts[0]->color.g;
		currentPacketType[currentPos].color.b = faces[i].verts[0]->color.b;
		currentPacketType[currentPos].color.a = faces[i].verts[0]->color.a;
		currentPacketType[currentPos].position.x = static_cast<float>(faces[i].verts[0]->position.x);
		currentPacketType[currentPos].position.y = static_cast<float>(faces[i].verts[0]->position.y);
		currentPacketType[currentPos].position.z = static_cast<float>(faces[i].verts[0]->position.z);
		currentPacketType[currentPos].normal.x = static_cast<float>(faces[i].verts[0]->normal.x);
		currentPacketType[currentPos].normal.y = static_cast<float>(faces[i].verts[0]->normal.y);
		currentPacketType[currentPos].normal.z = static_cast<float>(faces[i].verts[0]->normal.z);
		currentPacketType[currentPos].texture.x = static_cast<float>(faces[i].textures[0]->x);
		currentPacketType[currentPos].texture.y = -static_cast<float>(faces[i].textures[0]->y);
		currentPos++;

		currentPacketType[currentPos].color.r = faces[i].verts[1]->color.r;
		currentPacketType[currentPos].color.g = faces[i].verts[1]->color.g;
		currentPacketType[currentPos].color.b = faces[i].verts[1]->color.b;
		currentPacketType[currentPos].color.a = faces[i].verts[1]->color.a;
		currentPacketType[currentPos].position.x = static_cast<float>(faces[i].verts[1]->position.x);
		currentPacketType[currentPos].position.y = static_cast<float>(faces[i].verts[1]->position.y);
		currentPacketType[currentPos].position.z = static_cast<float>(faces[i].verts[1]->position.z);
		currentPacketType[currentPos].normal.x = static_cast<float>(faces[i].verts[1]->normal.x);
		currentPacketType[currentPos].normal.y = static_cast<float>(faces[i].verts[1]->normal.y);
		currentPacketType[currentPos].normal.z = static_cast<float>(faces[i].verts[1]->normal.z);
		currentPacketType[currentPos].texture.x = static_cast<float>(faces[i].textures[1]->x);
		currentPacketType[currentPos].texture.y = -static_cast<float>(faces[i].textures[1]->y);
		currentPos++;

		currentPacketType[currentPos].color.r = faces[i].verts[2]->color.r;
		currentPacketType[currentPos].color.g = faces[i].verts[2]->color.g;
		currentPacketType[currentPos].color.b = faces[i].verts[2]->color.b;
		currentPacketType[currentPos].color.a = faces[i].verts[2]->color.a;
		currentPacketType[currentPos].position.x = static_cast<float>(faces[i].verts[2]->position.x);
		currentPacketType[currentPos].position.y = static_cast<float>(faces[i].verts[2]->position.y);
		currentPacketType[currentPos].position.z = static_cast<float>(faces[i].verts[2]->position.z);
		currentPacketType[currentPos].normal.x = static_cast<float>(faces[i].verts[2]->normal.x);
		currentPacketType[currentPos].normal.y = static_cast<float>(faces[i].verts[2]->normal.y);
		currentPacketType[currentPos].normal.z = static_cast<float>(faces[i].verts[2]->normal.z);
		currentPacketType[currentPos].texture.x = static_cast<float>(faces[i].textures[2]->x);
		currentPacketType[currentPos].texture.y = -static_cast<float>(faces[i].textures[2]->y);
		currentPos++;
	}

	if( VBO_IBO )
	{

		// Load the Render Packets to the graphics card buffers
		RenderPacket* packArray;
		std::string texture;
		int size;

		for( auto iter = RenderPackages.begin(); iter != RenderPackages.end(); iter++ )
		{
			// Vertex data
			texture = (iter->first);
			packArray = (iter->second);
			size = RenderPackagesSize[ texture ];

			glBindBuffer_ptr( GL_ARRAY_BUFFER, VBOs[texture] );
			glBufferData_ptr( GL_ARRAY_BUFFER, sizeof(RenderPacket) * ( size * 3 ), packArray, GL_DYNAMIC_DRAW );

			// Index data
			glBindBuffer_ptr( GL_ELEMENT_ARRAY_BUFFER, IBOs[ texture ] );
			glBufferData_ptr( GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ( size * 3 ), RenderPackagesIndices[ texture ], GL_DYNAMIC_DRAW );
		}

	}
}


void Model::draw( Point3D position, Size3D scale, Rotation3D rotation, Color4F color, GLuint overrideTexture )
{
	std::string currentTexture = "";
	Material* currentMaterial = NULL;

	// Save the old matrix
	glPushMatrix();

	// Move and rotate into position
	glTranslated( position.x, position.y, position.z );
	glScaled( scale.w, scale.h, scale.d );
	glRotated(rotation.x, 1.0f, 0.0f, 0.0f);
	glRotated(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotated(rotation.z, 0.0f, 0.0f, 1.0f);

		// Switch rendering between the diffrent materials
		RenderPacket* packArray;
		std::string texture;

		// Material defaults
		int specComp = 0;
		float specular[] = { 0.0, 0.0, 0.0, 1.0 };
		float emission[] = { 0.0, 0.0, 0.0, 0.0 };
		float ambient[] = { 0.0, 0.0, 0.0, 1.0 };
		float diffuse[] = { color.r, color.g, color.b, color.a };

		for( auto iter = RenderPackages.begin(); iter != RenderPackages.end(); iter++ )
		{

			// Render Packages
			texture = (iter->first);
			packArray = (iter->second);
			currentMaterial = MaterialContainer::getMaterial( (iter->first) );

			// Change materials
			if( currentMaterial != NULL )
			{

					if( currentMaterial->texture != -1 )
					{
						if( !glIsEnabled( GL_TEXTURE_2D ) )
							glEnable( GL_TEXTURE_2D );

						if( overrideTexture != -1 )
							glBindTexture( GL_TEXTURE_2D, overrideTexture );
						else
							glBindTexture( GL_TEXTURE_2D, currentMaterial->texture );

					}
					else
					{
						if( glIsEnabled( GL_TEXTURE_2D ) )
							glDisable( GL_TEXTURE_2D );
					}

					specComp = currentMaterial->specularExponent;

					specular[0] = currentMaterial->specularity.r;
					specular[1] = currentMaterial->specularity.g;
					specular[2] = currentMaterial->specularity.b;
					specular[3] = currentMaterial->specularity.a;

					diffuse[0] = currentMaterial->diffuse.r + color.r;
					diffuse[1] = currentMaterial->diffuse.g + color.g;
					diffuse[2] = currentMaterial->diffuse.b + color.b;
					diffuse[3] = currentMaterial->diffuse.a + color.a;

					ambient[0] = currentMaterial->ambience.r;
					ambient[1] = currentMaterial->ambience.g;
					ambient[2] = currentMaterial->ambience.b;
					ambient[3] = currentMaterial->ambience.a;

					emission[0] = currentMaterial->emissive.r;
					emission[1] = currentMaterial->emissive.g;
					emission[2] = currentMaterial->emissive.b;
					emission[3] = currentMaterial->emissive.a;
			}
			else
			{
				if( overrideTexture != -1 )
				{
					glEnable( GL_TEXTURE_2D );
					glBindTexture( GL_TEXTURE_2D, overrideTexture );
				}
				else
				{
					glDisable( GL_TEXTURE_2D );
				}

				specular[0] = 0.f;
				specular[1] = 0.f;
				specular[2] = 0.f;
				specular[3] = 1.f;

				diffuse[0] = color.r;
				diffuse[1] = color.g;
				diffuse[2] = color.b;
				diffuse[3] = color.a;

				emission[0] = 0.f;
				emission[1] = 0.f;
				emission[2] = 0.f;
				emission[3] = 1.f;

			}

			// Set material properties
			glMateriali(  GL_FRONT, GL_SHININESS, specComp );
			glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
			glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
			glMaterialfv( GL_FRONT, GL_EMISSION, emission );
			glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
					
			glBegin( GL_POINT );
				glColor4f( diffuse[0], diffuse[1], diffuse[2], diffuse[3] ); // Dirty color hack
			glEnd();
				
			if( VBO_IBO )
			{
				glBindBuffer_ptr(GL_ARRAY_BUFFER, VBOs[ texture ] );
				glBindBuffer_ptr(GL_ELEMENT_ARRAY_BUFFER, IBOs[ texture ] );
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState (GL_TEXTURE_COORD_ARRAY);
			//glEnableClientState(GL_COLOR_ARRAY ); // Disabled the color array because of color hack

			if( VBO_IBO )
			{
				glTexCoordPointer( 2, GL_FLOAT, sizeof(RenderPacket), BUFFER_OFFSET(28) );
				glNormalPointer( GL_FLOAT, sizeof(RenderPacket), BUFFER_OFFSET(12) );
				//glColorPointer( 4, GL_FLOAT, sizeof(RenderPacket), BUFFER_OFFSET(36) );
				glVertexPointer(3, GL_FLOAT, sizeof(RenderPacket), BUFFER_OFFSET(0) );

				glDrawElements(GL_TRIANGLES, RenderPackagesSize[texture]*3, GL_UNSIGNED_INT, NULL);
			}
			else
			{
				glTexCoordPointer( 2, GL_FLOAT, sizeof(RenderPacket), &packArray[0].texture.x );
				glNormalPointer( GL_FLOAT, sizeof(RenderPacket), &packArray[0].normal.x );
				//glColorPointer( 4, GL_FLOAT, sizeof(RenderPacket), &packArray[0].color.r );
				glVertexPointer(3, GL_FLOAT, sizeof(RenderPacket), &packArray[0].position.x );

				glDrawElements(GL_TRIANGLES, RenderPackagesSize[texture]*3, GL_UNSIGNED_INT, RenderPackagesIndices[texture] );
			}

			//glDisableClientState(GL_COLOR_ARRAY );
			glDisableClientState (GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

	}

	glPopMatrix();
}

void Model::calcNormals( bool force )
{

	// Clear out all of the normals
	for( int i = 0; i < amountOfVerts; i++ )
	{
		verts[i].normal.clear();
	}

	// Add face normals to all of the verts
	for( int i = 0; i < amountOfFaces; i++ )
	{
		Point3D normal = Math::calcNormal( faces[i].verts[0]->position, faces[i].verts[1]->position, faces[i].verts[2]->position );

		faces[i].verts[0]->normal.x += normal.x;
		faces[i].verts[0]->normal.y += normal.y;
		faces[i].verts[0]->normal.z += normal.z;

		faces[i].verts[1]->normal.x += normal.x;
		faces[i].verts[1]->normal.y += normal.y;
		faces[i].verts[1]->normal.z += normal.z;

		faces[i].verts[2]->normal.x += normal.x;
		faces[i].verts[2]->normal.y += normal.y;
		faces[i].verts[2]->normal.z += normal.z;

	}
	
	// Normalize vectors
	for( int i = 0; i < amountOfVerts; i++ )
		verts[i].normal.Normalize();
	
}


void Model::drawNormals( Point3D position, Size3D scale, Rotation3D rotation, double length )
{
	// Save the old matrix
	glPushMatrix();

	// Move and rotate into position
	glTranslated( position.x, position.y, position.z );
	glRotated(rotation.x, 1.0f, 0.0f, 0.0f);
	glRotated(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotated(rotation.z, 0.0f, 0.0f, 1.0f);

	for( int i = 0; i < amountOfVerts; i++ )
		GLDebug::drawPoint3D( verts[i].position, scale, verts[i].normal, length );

	glPopMatrix();
}


void Model::drawPoints( Point3D position, Size3D scale, Rotation3D rotation, double length )
{

	// Save the old matrix
	glPushMatrix();

	// Move and rotate into position
	glTranslated( position.x, position.y, position.z );
	glRotated(rotation.x, 1.0f, 0.0f, 0.0f);
	glRotated(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotated(rotation.z, 0.0f, 0.0f, 1.0f);

	for( int i = 0; i < amountOfVerts; i++ )
		GLDebug::drawPoint3D( verts[i].position, scale, length );

	glPopMatrix();
}

Size3D Model::getSize()
{
	return size;
}