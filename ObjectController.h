/***

Object Controller: Allows functionality for dynamically generated StaticObjects

***/

#ifndef OBJECT_CONTROLLER_H
#define OBJECT_CONTROLLER_H

#include <vector>
#include "StaticObject.h"
#include "DisplayObject.h"
#include "OctTree.h"

class ObjectController
{
private:
	// Vector of created object controllers
	static std::vector< ObjectController* > controllers;

	static Point3D renderPos;
	static Size3D renderDistance;

	// Will expand the ctrObjects array
	void ExpandObjectsArray();

	// PtrPtr to array of static objects
	std::list< DisplayObject* > ctrObjects;
	OctTree soOctTree;
	//StaticObject** ctrObjects;

	// Size / number of elements in the array
	int size;

	// Max size of the array
	int maxSize;

public:

	ObjectController();

	// Will cycle through all controllers and clear them
	static void clearAllControllers();

	static void UpdateRenderPosition( Point3D position );
	static void SetRenderDistance( double range );

	// Adds newObject to the controller
	bool addObject( DisplayObject* newObject );

	// Moves all the the objects in the controller
	void moveObjects();

	// Draws all objects in the controller
	void drawObjects();

	// Draws all objects in the controller debug information
	void drawObjectsDebugInfo();

	// Handles events for all objects in the controller
	void eventObjects( SDL_Event* e );

	// Pauses objects internal timers
	void PauseObjects();

	// Unpauses objects internal timers
	void UnpauseObjects();

	// Cycles through the controllers list and deletes any objects specified for garbage collection
	void garbageObjects();

	// Deletes everything held by the controller
	void clear();
};

#endif // !OBJECT_CONTROLLER_H
