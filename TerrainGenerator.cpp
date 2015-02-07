#include "TerrainGenerator.h"

std::vector<TerrainGenerator*> TerrainGenerator::TerrainGenerators;

TerrainGenerator::TerrainGenerator()
{
	for( int i = 0; i < TERRAIN_WIDTH; i++ )
	{
		for( int j = 0; j < TERRAIN_DEPTH; j++ )
		{
			chunkGenerated[i][j] = NULL;
		}
	}
	memoryHasBeenAllocated = false;

	playerPosition.x = 150.5f;
	playerPosition.y = 0.f;
	playerPosition.z = 150.5f;

	chunkSize = 200;
	chunkDetail = 20;

	chunkWidthOffset = TERRAIN_WIDTH/2.0*chunkSize;
	chunkDepthOffset = TERRAIN_DEPTH/2.0*chunkSize;

	chunks = 0;
	viewDistance = 600;

	TerrainGenerators.push_back(this);
}

void TerrainGenerator::ClearAllTerrainGenerators()
{
	for( auto iter = TerrainGenerators.begin(); iter != TerrainGenerators.end(); iter++ )
	{
		(*iter)->Clear();
	}
}

void TerrainGenerator::draw()
{
	currentChunks.drawObjects();
}

void TerrainGenerator::UpdatePlayerPosition( Point3D position)
{
	// Updates plater position
	playerPosition.x = static_cast<float>( position.x / chunkSize + ( ( chunkWidthOffset + chunkSize / 2.0 ) / chunkSize ) );
	playerPosition.y = static_cast<float>( position.y / chunkSize );
	playerPosition.z = static_cast<float>( position.z / chunkSize + ( ( chunkDepthOffset + chunkSize / 2.0 ) / chunkSize ) );

	int floorX = static_cast<int>( floor( playerPosition.x ) );
	int floorZ = static_cast<int>( floor( playerPosition.z ) );

	// If the player moves over a tile, 
	if( floor(playerPosition.x) != playerFloorPos.x || floor(playerPosition.z) != playerFloorPos.z || !memoryHasBeenAllocated )
	{
		// The range to set hidden chunks
		int maxRange = static_cast<int>( viewDistance / chunkSize / 2.0 );

		// Show all of the tiles in range,
		// generate chunks if needed
		for( int i = floorX-(maxRange); i < floorX+(maxRange); i++)
		{
			for( int j = floorZ-maxRange; j < floorZ+maxRange; j++)
			{
				if( i > 0 && j > 0 && i < TERRAIN_WIDTH && j < TERRAIN_DEPTH )
				{
					GenerateChunk( i, j ); // Does not check for i, j < 0 || i, j > MAX_WIDTH/HEIGHT
				}
			}
		}

	}

	// Set the floor variables to only calculate new chunks / visiblilty at tile breaks
	playerFloorPos.x = floorX;
	playerFloorPos.z = floorZ;
}

void TerrainGenerator::GenerateChunk( int x, int z, Model* chunk )
{
	if( chunkGenerated[x][z] != NULL )
		return;

	// Store the new chunk in the model container if not
	// provided one
	if( chunk == NULL )
	{
		std::stringstream ss;
		ss << "chunk" << chunks++;
		ModelContainer::addModel( DynamicMesh::generatePlain( chunkSize, chunkSize, chunkDetail, MaterialContainer::getMaterial( "Mat.Grass" ) ), ss.str() );
		chunk = ModelContainer::getModel( ss.str() );
	}
	else
		chunks++;

	// Creat the new chunk object
	StaticObject* newChunk = new StaticObject;

	newChunk->setModel( chunk );
	newChunk->setPosition( ( x * chunkSize ) - ( chunkWidthOffset ), -2.0, ( z * chunkSize ) - ( chunkDepthOffset ) );
	currentChunks.addObject( newChunk );
	chunkGenerated[x][z] = newChunk;

	memoryHasBeenAllocated = true;
}

StaticObject* TerrainGenerator::GetCurrentChunk()
{
	int xPos = static_cast<int>(playerPosition.x);
	int zPos = static_cast<int>(playerPosition.z);

	if( xPos < 0 || xPos > TERRAIN_WIDTH || zPos < 0 || zPos > TERRAIN_DEPTH )
		return NULL;

	return chunkGenerated[xPos][zPos];
}

Point3F TerrainGenerator::GetPlayerPosition()
{
	return Point3F( static_cast<float>( ( playerPosition.x - 150.5f ) * chunkSize ), static_cast<float>( playerPosition.y * chunkSize ), static_cast<float>( ( playerPosition.z - 150.5f ) * chunkSize ) );
}


// Set / Get the chunk size of the world
double TerrainGenerator::GetChunkSize()
{
	return chunkSize;
}

void TerrainGenerator::SetChunkSize( double chunkSize )
{
	if( chunkSize < 0.0 )
		chunkSize = 1.0;

	this->chunkSize = chunkSize;
	chunkWidthOffset = TERRAIN_WIDTH/2.0*chunkSize;
	chunkDepthOffset = TERRAIN_DEPTH/2.0*chunkSize;

	viewDistance = static_cast<int>( chunkSize * 4 );
}

// Set / Get the chunk detail of then world
int TerrainGenerator::GetChunkDetail()
{
	return chunkDetail;
}

void TerrainGenerator::SetChunkDetail( int chunkDetail )
{
	this->chunkDetail = chunkDetail;
}

void TerrainGenerator::SetGenerationDistance( int viewGenDistance )
{
	viewDistance = viewGenDistance;
}

std::vector<StaticObject*> TerrainGenerator::GetClosestChunks()
{
	std::vector<StaticObject*> stsObjsReturn;

		int maxRange = static_cast<int>( viewDistance / chunkSize );

		for( int i = playerFloorPos.x-(maxRange); i < playerFloorPos.x+(maxRange); i++)
		{
			for( int j = playerFloorPos.z-maxRange; j < playerFloorPos.z+maxRange; j++)
			{
				if( i > 0 && j > 0 && i < TERRAIN_WIDTH && j < TERRAIN_DEPTH && chunkGenerated[i][j] != NULL )
				{
					stsObjsReturn.push_back( chunkGenerated[i][j] );
				}
			}
		}

	return stsObjsReturn;
}


bool TerrainGenerator::SaveWorld( std::string path )
{
	std::string newLine = "\n";
	std::ofstream worldFile;
	worldFile.open ( path );

	worldFile << "#Created by Cubic Engine, ©AW-Games.com 2012-2013, All Rights Reserved" << newLine;
	worldFile << "worldDetail: " << chunkDetail << newLine;
	worldFile << "worldSize: " << chunkSize << newLine;
	worldFile << "viewDistance: " << viewDistance << newLine;
	worldFile << "playerPos: " << playerPosition.x << " " << playerPosition.y << " " << playerPosition.z << newLine;

	for( int i = 0; i < TERRAIN_WIDTH; i++ )
	{
		for( int j = 0; j < TERRAIN_DEPTH; j++ )
		{
			if( chunkGenerated[i][j] != NULL )
			{
				worldFile << i << ',' << j << ' ' << chunkGenerated[i][j]->getModel()->Name() << newLine;
			}
		}
	}

	worldFile.close();
	return true;
}

bool TerrainGenerator::LoadWorld( std::string path )
{
	std::string line;
	std::ifstream myfile ( path );

	int xPos = 0;
	int zPos = 0;
	Model* chunk = NULL;
	std::string modelName = "";

	std::string rootFolder = "export/";

	// Get number of verts in the file
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline ( myfile, line );

			if( StringFunctions::beginsWith( line, "#" ) )
				continue;

			if( StringFunctions::beginsWith( line, "worldDetail" ) )
			{
				chunkDetail = atoi( StringFunctions::getPram( line, 2, ' ' ).c_str() );
				continue;
			}

			if( StringFunctions::beginsWith( line, "worldSize" ) )
			{
				chunkSize = atoi( StringFunctions::getPram( line, 2, ' ' ).c_str() );
				chunkWidthOffset = TERRAIN_WIDTH/2.0*chunkSize;
				chunkDepthOffset = TERRAIN_DEPTH/2.0*chunkSize;
				continue;
			}

			if( StringFunctions::beginsWith( line, "viewDistance" ) )
			{
				viewDistance = atoi( StringFunctions::getPram( line, 2, ' ' ).c_str() );
				continue;
			}

			if( StringFunctions::beginsWith( line, "playerPos" ) )
			{
				playerPosition.x = static_cast<float>( atof( StringFunctions::getPram( line, 2, ' ' ).c_str() ) );
				playerPosition.y = static_cast<float>( atof( StringFunctions::getPram( line, 3, ' ' ).c_str() ) );
				playerPosition.z = static_cast<float>( atof( StringFunctions::getPram( line, 4, ' ' ).c_str() ) );
				continue;
			}

			if( StringFunctions::getNumOfPrams( line, ',' ) == 2 )
			{
				xPos = atoi( StringFunctions::getPram( line, 1, ',' ).c_str() ) ;
				zPos = atoi( StringFunctions::getPram( StringFunctions::getPram( line, 1, ' ' ), 2, ',' ).c_str() ) ;
				modelName = StringFunctions::getPram( line, 2, ' ' );

				if( !ModelContainer::addModel( rootFolder + modelName + ".obj", modelName, true, false ) )
					return false; // Error out if any of the models fail to load

				chunk = ModelContainer::getModel( modelName );

				GenerateChunk( xPos, zPos, chunk );

			}

		}
	}
	else
	{
		return false;
	}

	return true;
}

void TerrainGenerator::Clear()
{

	if( memoryHasBeenAllocated )
	{
		currentChunks.clear();
	}

	for( int i = 0; i < TERRAIN_WIDTH; i++ )
	{
		for( int j = 0; j < TERRAIN_DEPTH; j++ )
		{
			chunkGenerated[i][j] = NULL;
		}
	}
	memoryHasBeenAllocated = false;

	ModelContainer::clear( "chunk" );

	chunks = 0;

}