#include "MaterialContainer.h"

std::unordered_map<std::string,Material*> MaterialContainer::materials;
Material* MaterialContainer::lastMaterial = NULL;

bool MaterialContainer::loadMaterialFile( std::string path )
{

	std::string line;
	std::ifstream myfile ( path );

	Material* currentMaterial = NULL;

	std::string texture = "";
	std::string textureName = "";
	std::string textureFullPath = "";

	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline ( myfile, line );

			// Ignore empty lines
			if( line.size() == 0 )
				continue;

			// Get rid of double spaces
			StringFunctions::spliceOut( line, "  ", " " );

			// Ignore comments
			if( StringFunctions::beginsWith( line, "#" ) )
				continue;

			// Create new materials
			if( StringFunctions::beginsWith( line, "newmtl" ) )
			{
					currentMaterial = new Material;
					currentMaterial->name = StringFunctions::getPram( line, 2, ' ');
					materials[currentMaterial->name] = currentMaterial;
					continue;
			}

			// Dont allow past this point unless there is a bound material
			if( currentMaterial == NULL )
				continue;

			// Specular Exponent
			if( StringFunctions::beginsWith( line, "Ns" ) )
			{
				currentMaterial->specularExponent = atoi( StringFunctions::getPram( line, 2, ' ' ).c_str() );
				continue;
			}

			// Material Ambience
			if( StringFunctions::beginsWith( line, "Ka" ) )
			{
				currentMaterial->ambience.r = static_cast<float>(atof( StringFunctions::getPram( line, 2, ' ' ).c_str() ));
				currentMaterial->ambience.g = static_cast<float>(atof( StringFunctions::getPram( line, 3, ' ' ).c_str() ));
				currentMaterial->ambience.b = static_cast<float>(atof( StringFunctions::getPram( line, 4, ' ' ).c_str() ));
				continue;
			}

			// Material Diffuse
			if( StringFunctions::beginsWith( line, "Kd" ) )
			{
				currentMaterial->diffuse.r = static_cast<float>(atof( StringFunctions::getPram( line, 2, ' ' ).c_str() ));
				currentMaterial->diffuse.g = static_cast<float>(atof( StringFunctions::getPram( line, 3, ' ' ).c_str() ));
				currentMaterial->diffuse.b = static_cast<float>(atof( StringFunctions::getPram( line, 4, ' ' ).c_str() ));
				continue;
			}

			// Material Specular
			if( StringFunctions::beginsWith( line, "Ks" ) )
			{
				currentMaterial->specularity.r = static_cast<float>(atof( StringFunctions::getPram( line, 2, ' ' ).c_str() ));
				currentMaterial->specularity.g = static_cast<float>(atof( StringFunctions::getPram( line, 3, ' ' ).c_str() ));
				currentMaterial->specularity.b = static_cast<float>(atof( StringFunctions::getPram( line, 4, ' ' ).c_str() ));
				continue;
			}

			// Dissolve
			if( StringFunctions::beginsWith( line, "d" ) )
			{
				currentMaterial->dissolve = static_cast<float>( atof( StringFunctions::getPram( line, 2, ' ' ).c_str() ) );
				continue;
			}

			// Illumination Model
			if( StringFunctions::beginsWith( line, "illum" ) )
			{
				currentMaterial->illuminationModel = atoi( StringFunctions::getPram( line, 2, ' ' ).c_str() );
				continue;
			}

			// Texture
			if( StringFunctions::beginsWith( line, "map_Kd" ) )
			{
				StringFunctions::removeFront( line, 7 );
				textureFullPath = line;
				StringFunctions::spliceOut( line, "\\", " " );
				texture = StringFunctions::getPram( line, StringFunctions::getNumOfPrams( line, ' ' ), ' ' );
				textureName = StringFunctions::getPram( texture, 1, '.' );

				if( GLTextureContainer::hasTexture( textureName ) )
				{
					currentMaterial->texture = GLTextureContainer::getTexture( textureName )->textureID;
					currentMaterial->textureName = textureName;
					continue;
				}

				if( GLTextureContainer::addTexture( textureFullPath, textureName ) != NULL )
				{
					currentMaterial->texture = GLTextureContainer::getTexture( textureName )->textureID;
					currentMaterial->textureName = textureName;
					continue;
				}

				if( GLTextureContainer::addTexture( "obj/"+texture, textureName ) != NULL )
				{
					currentMaterial->texture = GLTextureContainer::getTexture( textureName )->textureID;
					currentMaterial->textureName = textureName;
					continue;
				}

				if( GLTextureContainer::addTexture( "image/"+texture, textureName ) != NULL )
				{
					currentMaterial->texture = GLTextureContainer::getTexture( textureName )->textureID;
					currentMaterial->textureName = textureName;
					continue;
				}

				currentMaterial->texture = -1;
				currentMaterial->textureName = "";
				continue;
			}
		}
	}
	

	return true;
}

Material* MaterialContainer::NewMaterial( std::string name )
{
	if( hasMaterial( name ) )
		return materials[name];

	materials[name] = new Material;
	materials[name]->name = name;

	lastMaterial = materials[name];

	return materials[name];
}

bool MaterialContainer::ExportMaterials( std::string path )
{
	std::unordered_map<std::string, Material*>::iterator iter;
	std::string newLine = "\n";
	std::ofstream materialFile;

	materialFile.open ( path );
	materialFile << "#Created by Cubic Engine, ©AW-Games.com 2012-2013, All Rights Reserved" << newLine << newLine;

	for( iter = materials.begin(); iter != materials.end(); iter++ )
	{

		materialFile << "newmtl " << iter->first << newLine;
		materialFile << "Ns " << iter->second->specularExponent << newLine;
		materialFile << "Ka " << iter->second->ambience.r << ' ' << iter->second->ambience.g << ' ' << iter->second->ambience.b << newLine;
		materialFile << "Kd " << iter->second->diffuse.r << ' ' << iter->second->diffuse.g << ' ' << iter->second->diffuse.b << newLine;
		materialFile << "Ks " << iter->second->specularity.r << ' ' << iter->second->specularity.g << ' ' << iter->second->specularity.b << newLine;
		if( iter->second->textureName != "" )
			materialFile << "map_Kd " << iter->second->textureName << newLine;
		materialFile << newLine;
	}

	materialFile.close();
	return true;
}

void MaterialContainer::addMaterial( Material* material, std::string name )
{
	if( hasMaterial( name ) || material == NULL )
		return;

	lastMaterial = material;
	materials[name] = material;
}

// Sets the diffuse of the last material
void MaterialContainer::SetDiffuse( Color4F diffuse )
{
	if( lastMaterial == NULL )
		return;

	lastMaterial->diffuse.r = diffuse.r;
	lastMaterial->diffuse.g = diffuse.g;
	lastMaterial->diffuse.b = diffuse.b;
	lastMaterial->diffuse.a = diffuse.a;
}

// Sets the ambience of the last material
void MaterialContainer::SetAmbience( Color4F ambience )
{
	if( lastMaterial == NULL )
		return;

	lastMaterial->ambience.r = ambience.r;
	lastMaterial->ambience.g = ambience.g;
	lastMaterial->ambience.b = ambience.b;
	lastMaterial->ambience.a = ambience.a;
}

// Sets the emissive of the last material
void MaterialContainer::SetEmissive( Color4F emissive )
{
	if( lastMaterial == NULL )
		return;

	lastMaterial->emissive.r = emissive.r;
	lastMaterial->emissive.g = emissive.g;
	lastMaterial->emissive.b = emissive.b;
	lastMaterial->emissive.a = emissive.a;
}

// Sets the specularity of the last material
void MaterialContainer::SetSpecularity( Color4F specularity )
{
	if( lastMaterial == NULL )
		return;

	lastMaterial->specularity.r = specularity.r;
	lastMaterial->specularity.g = specularity.g;
	lastMaterial->specularity.b = specularity.b;
	lastMaterial->specularity.a = specularity.a;
}

// Sets the specularity exponent
void MaterialContainer::SetSpecularityExponent( int specExponent )
{
	if( lastMaterial == NULL )
		return;

	lastMaterial->specularExponent = specExponent;
}

// Sets the texture of the material
void MaterialContainer::SetTexture( Texture* texture, std::string name )
{
	if( lastMaterial == NULL )
		return;

	lastMaterial->texture = texture->textureID;
	lastMaterial->textureName = name;
}
// -Associated with GLTextureContainer
void MaterialContainer::SetTexture( std::string textureName )
{
	if( lastMaterial == NULL || !GLTextureContainer::hasTexture( textureName ) )
		return;

	lastMaterial->textureName = textureName;
	lastMaterial->texture = GLTextureContainer::getTexture( textureName )->textureID;
}

void MaterialContainer::clear()
{
}

Material* MaterialContainer::getMaterial( std::string name )
{
	if( materials.find( name ) == materials.end() )
		return NULL;

	return materials[name];
}

bool MaterialContainer::hasMaterial( std::string name )
{
	if( materials.find( name ) == materials.end() )
		return false;
	return true;
}