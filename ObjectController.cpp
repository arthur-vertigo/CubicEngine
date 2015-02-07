#include "ObjectController.h"

std::vector< ObjectController* > ObjectController::controllers;
Point3D ObjectController::renderPos;
Size3D ObjectController::renderDistance;

ObjectController::ObjectController()
{
	soOctTree.size.w = 1000000;
	soOctTree.size.h = 1000000;
	soOctTree.size.d = 1000000;
	controllers.push_back( this );
	maxSize = 10;
	size = 0;
}

void ObjectController::UpdateRenderPosition( Point3D position )
{
	renderPos = position;
}
void ObjectController::SetRenderDistance( double range )
{
	renderDistance.w = range;
	renderDistance.h = range;
	renderDistance.d = range;
}

void ObjectController::ExpandObjectsArray()
{
}

void ObjectController::clear()
{
	for(auto iter = ctrObjects.begin(); iter != ctrObjects.end(); )
	{
		delete (*iter++);
	}
	ctrObjects.clear();
}

void ObjectController::clearAllControllers()
{
	for( auto iter = controllers.begin(); iter != controllers.end(); iter++ )
		(*iter)->clear();

}

bool ObjectController::addObject( DisplayObject* newObject )
{
	if( newObject == NULL )
		return false;

	if( newObject->type == "so" )
	{
		soOctTree.Insert( dynamic_cast<WorldObject*>( newObject ) );
	}
	else
	{
		ctrObjects.push_back( newObject );
	}
	size++;

	return true;
}

void ObjectController::moveObjects()
{
	for( auto iter = ctrObjects.begin(); iter != ctrObjects.end(); iter++ )
		(*iter)->move();

	if( soOctTree.length != 0 )
	{
		auto vect = soOctTree.GetObjects( renderPos, renderDistance );
		for( auto iter = vect.begin(); iter != vect.end(); iter++ ) 
		{
			dynamic_cast<StaticObject*>((*iter))->move();
		}

	}
}

void ObjectController::drawObjects()
{
	for( auto iter = ctrObjects.begin(); iter != ctrObjects.end(); iter++ )
		(*iter)->draw();

	if( soOctTree.length != 0 )
	{
		auto vect = soOctTree.GetObjects( renderPos, renderDistance );
		for( auto iter = vect.begin(); iter != vect.end(); iter++ ) 
		{
			dynamic_cast<StaticObject*>((*iter))->draw();
		}

	}
}

void ObjectController::drawObjectsDebugInfo()
{
	for( auto iter = ctrObjects.begin(); iter != ctrObjects.end(); iter++ )
		(*iter)->drawDebugInfo( .5 );
}

void ObjectController::eventObjects( SDL_Event* e )
{
	for( auto iter = ctrObjects.begin(); iter != ctrObjects.end(); iter++ )
		(*iter)->handleEvent( *e );

	if( soOctTree.length != 0 )
	{
		auto vect = soOctTree.GetObjects( renderPos, renderDistance );
		for( auto iter = vect.begin(); iter != vect.end(); iter++ ) 
		{
			dynamic_cast<StaticObject*>((*iter))->handleEvent( *e );
		}

	}
}

// Pauses objects internal timers
void ObjectController::PauseObjects()
{
	for( auto iter = ctrObjects.begin(); iter != ctrObjects.end(); iter++ )
		(*iter)->Pause();

	if( soOctTree.length != 0 )
	{
		auto vect = soOctTree.GetObjects( renderPos, renderDistance );
		for( auto iter = vect.begin(); iter != vect.end(); iter++ ) 
		{
			dynamic_cast<StaticObject*>((*iter))->Pause();
		}

	}
}

// Unpauses objects internal timers
void ObjectController::UnpauseObjects()
{
	for( auto iter = ctrObjects.begin(); iter != ctrObjects.end(); iter++ )
		(*iter)->Unpause();

	if( soOctTree.length != 0 )
	{
		auto vect = soOctTree.GetObjects( renderPos, renderDistance );
		for( auto iter = vect.begin(); iter != vect.end(); iter++ ) 
		{
			dynamic_cast<StaticObject*>((*iter))->Unpause();
		}

	}
}

void ObjectController::garbageObjects()
{
}
