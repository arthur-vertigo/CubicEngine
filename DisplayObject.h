/***

Display Object: Defines a basic display object
				-Contains the name of the object
				-Contains the type of the object
				-Holds garbage collection variables
				-Holds generic display information

***/

#ifndef DISPLAY_OBJECT_H
#define DISPLAY_OBJECT_H

#include <string>
#include "SDL.h"


class DisplayObject
{
public:
	DisplayObject()
	{
		name = "";
		type = "";
		garbageCollect = false;
		visible = true;
		hasLighting = true;
		hasFog = true;
	}

	// Demographics
	std::string name;
	std::string type;
	bool garbageCollect;
	bool visible;
	bool hasLighting;
	bool hasFog;

	// Virtual functions
	virtual void draw() = 0;
	virtual void move() = 0;
	virtual void drawDebugInfo( double length ) = 0;
	virtual void handleEvent( SDL_Event& e ) = 0;
	virtual void Pause() = 0;
	virtual void Unpause() = 0;
};

#endif // !DISPLAY_OBJECT_H
