#include "GLTextureContainer.h"

std::unordered_map<std::string,Texture*> GLTextureContainer::GLTextures;

bool isPng( std::string path )
{
	int length = path.size();
	if(  path[length-3] == 'p' && path[length-2] == 'n' && path[length-1] == 'g' )
		return true;
	return false;
}

Texture* GLTextureContainer::addTexture( std::string path, std::string name )
{
	if( hasTexture( name ) )
		return GLTextures[name];

	SDL_Surface* tempSurface;
	tempSurface = IMG_Load( path.c_str() );

	if( !tempSurface )
		return NULL;

	GLTextures[name] = new Texture;

	glGenTextures( 1, &GLTextures[name]->textureID );
	glBindTexture( GL_TEXTURE_2D, GLTextures[name]->textureID );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	if( isPng( path ) ) // Shitty method of determining alpha images
		glTexImage2D( GL_TEXTURE_2D, 0, 4, tempSurface->w, tempSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempSurface->pixels );
	else
		glTexImage2D( GL_TEXTURE_2D, 0, 3, tempSurface->w, tempSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tempSurface->pixels );

	GLTextures[name]->textureSize.w = tempSurface->w;
	GLTextures[name]->textureSize.h = tempSurface->h;

	SDL_FreeSurface( tempSurface );
	return GLTextures[name];
}

void GLTextureContainer::clear()
{
	for( auto iter = GLTextures.begin(); iter != GLTextures.end(); iter++ )
	{
		glDeleteTextures( 1, &iter->second->textureID );
	}
	GLTextures.clear();
}

Texture* GLTextureContainer::getTexture( std::string name )
{
	if( GLTextures.find( name ) == GLTextures.end() )
		return NULL;

	return GLTextures[name];
}

bool GLTextureContainer::hasTexture( std::string name )
{
	if( GLTextures.find( name ) == GLTextures.end() )
		return false;
	return true;
}