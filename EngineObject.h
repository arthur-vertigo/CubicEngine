/***

Engine Object: Defines portions of the engine
                -Dont know if this class is needed

***/

#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H


#include <string>

class EngineObject
{
public:
	EngineObject()
	{
		name = "";
	}

	std::string name;
};

#endif // !ENGINE_OBJECT_H
