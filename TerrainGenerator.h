/***

Terrain Generator: Will dynamicaly generate terrain

***/
#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <vector>
#include <sstream>
#include <cmath>
#include <thread>
#include "Metrics.h"
#include "Model.h"
#include "StaticObject.h"
#include "DynamicMesh.h"
#include "ModelContainer.h"
#include "ObjectController.h"

// The maximum size of the world is x chunks wide/deep
const int TERRAIN_WIDTH = 300;
const int TERRAIN_DEPTH = 300;

class TerrainGenerator : public DisplayObject
{
private:

	// Vector of all created Terrain Generators
	static std::vector<TerrainGenerator*> TerrainGenerators;

	// Flag for if any chunk memory has been allocated
	bool memoryHasBeenAllocated;

	// Defines the size of a chunk
	double chunkSize;

	// Defines the amount of face segmentations per chunk
	int chunkDetail;

	// Holds all of the chunks, preforms operations
	ObjectController currentChunks;

	// Holds the current amount of chunks
	int chunks;

	// 2d map for the position of chunks
	StaticObject* chunkGenerated[TERRAIN_WIDTH+10][TERRAIN_DEPTH+10];

	// Width and depth offset to make center of chunkGenerated map 0,0
	double chunkWidthOffset;
	double chunkDepthOffset;

	// Player position on the chunk map
	Point3F playerPosition;
	Point3I playerFloorPos; //Floored

	// The view distance of the player
	// -used in visibility calculations
	int viewDistance;

	// Will create a blank chunk at x,z
	void GenerateChunk( int x, int z, Model* chunk = NULL );
public:

	TerrainGenerator();

	// Clears all generated terrain generators
	static void ClearAllTerrainGenerators();

	// Updates the players position on the chunk map
	void UpdatePlayerPosition( Point3D position );
	Point3F GetPlayerPosition();

	// Draws all of the chunks
	void draw();
	void move() {};
	void handleEvent( SDL_Event& e ) {};
	void drawDebugInfo( double length ) {};
	void Pause() {};
	void Unpause() {};

	// Returns the current chunk the player is on
	StaticObject* GetCurrentChunk();

	// Returns all of the currently visible chunks
	std::vector<StaticObject*> GetClosestChunks();

	// Set / Get the chunk size of the world
	double GetChunkSize();
	void SetChunkSize( double chunkSize );

	// Set / Get the chunk detail of then world
	int GetChunkDetail();
	void SetChunkDetail( int chunkDetail );

	// Set / Get the view distance of the world
	void SetGenerationDistance( int viewGenDistance );

	// Save / loads the terrain information
	bool SaveWorld( std::string path );
	bool LoadWorld( std::string path );

	// Clears the world and model information
	// -Resets the world to default state
	void Clear();
};

#endif // !TERRAIN_GENERATOR_H
