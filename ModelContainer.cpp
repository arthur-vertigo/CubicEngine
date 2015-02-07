#include "ModelContainer.h"

std::unordered_map<std::string, Model*> Models;

bool ModelContainer::addModel( Model* newModel, std::string name )
{
	if( newModel == NULL || !newModel->hasLoaded() )
		return false;

	newModel->Name( name );
	Models[name] = newModel;
	return true;
}

bool ModelContainer::addModel( std::string path, std::string name, bool allowIncompleteLoading, bool sizeAndCenter )
{
	Model* newModel = new Model;

	if( getModel(name) != NULL )
		return false;

	newModel->load( path, allowIncompleteLoading, sizeAndCenter );

	if( !newModel->hasLoaded() )
	{
		delete newModel;
		return false;
	}

	newModel->Name( name );
	Models[name] = newModel;

	return true;
}

Model* ModelContainer::getModel( std::string name )
{
	if( Models.find( name ) == Models.end() )
		return NULL;

	return Models[name];
}

bool ModelContainer::HasModel( std::string name )
{
	if( Models.find( name ) == Models.end() )
		return false;
	return true;
}

void ModelContainer::clear( std::string compare )
{
	for( auto iter = Models.begin(); iter != Models.end();)
	{
		if( StringFunctions::beginsWith( iter->first, compare ) )
		{
			delete iter->second;
			Models.erase( iter++ );
		}
		else
		{
			iter++;
		}
	}
}

void ModelContainer::ClearVBO_IBOs()
{
	for( auto iter = Models.begin(); iter != Models.end(); iter++ )
	{
		iter->second->ClearVBO_IBOs();
	}
}

bool ModelContainer::ExportModels( std::string path, std::string compare )
{
	std::unordered_map<std::string,Model*>::iterator iter;
	std::string name = "";
	std::stringstream ss;

	for( iter = Models.begin(); iter != Models.end(); iter++ )
	{
		name = iter->second->Name();

		if( StringFunctions::beginsWith( name, compare ) )
		{
			ss.str("");
			ss << path << name << ".obj";
			iter->second->save( ss.str() );
		}
	}
	return true;
}