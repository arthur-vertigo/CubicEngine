/***

Game Classes: Classes designed specifically for the current game

***/

#ifndef GAME_CLASSES_H
#define GAME_CLASSES_H

#include "SDL.h"

#include "StaticObject2D.h"
#include "DisplayObject.h"
#include "HudObjects.h"
#include "Metrics.h"

// Tool-Belt class: Defines a dynamic interface for selecting diffrent mesh tools
class ToolBelt : public DisplayObject
{
private:
	// Textbox for rendering the bucket lables
	TextBox bucketLable;

	StaticObject2D* buckets;

	std::string* lables;

	// Graphic for the currently selected bucket
	StaticObject2D selectedBucket;

	// Position of the tool belt
	Point2D position;

	int amountOfBuckets;
	int selectedTool;
public:
	ToolBelt( int amountOfBuckets, BitmapFont* font, Texture* blankBucket, Texture* selectedBucket );
	~ToolBelt();

	// Position mutator
	void SetPosition( float x, float y );

	// Sets the currently selected tool
	void SetSelectedItem( int tool );

	// Sets the lable of a bucket
	void SetBucketLable( int bucket, std::string lable );

	// Returns the currently selected tool
	int GetSelectedTool();

	void draw();
	void move() {};
	void handleEvent( SDL_Event& e );
	void drawDebugInfo( double length ) {};
	void Pause() {};
	void Unpause() {};

};

// Player class defines the main player
class Player : public DisplayObject
{
private:
	Point3D position;
	Rotation3D rotation;
	Vector3D velocity;
	Size3D playerSize;

	double moveSpeed;
	double jumpHeight;
	Timer moveTimer;

	// The floor of the current position
	double floor;

	// True if the player is not effected by gravity
	bool flying;
	bool hasLanded;
	bool disableMouseMovement;

	// Hack to prevent player from falling until movement
	bool hasMoved;
public:
	Player();

	void SetPosition( Point3F position );
	Point3D GetPosition();
	Rotation3D GetRotation();

	bool isFlying();
	bool hasDisabledMouseMovement();

	void SetFloor( double y );

	void Pause();
	void Unpause();

	void draw() {};
	void move();
	void handleEvent( SDL_Event& e );
	void drawDebugInfo( double length ) {};
};

#endif // !GAME_CLASSES_H
