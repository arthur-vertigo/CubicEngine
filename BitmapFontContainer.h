/***

Bitmap Font Container: Contains bitmap fonts

***/

#ifndef BITMAP_FONT_CONTAINER_H
#define BITMAP_FONT_CONTAINER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

#include "BitmapFont.h"

class BitmapFontContainer
{
private:
	static std::unordered_map<std::string,BitmapFont*> BitmapFonts;
public:

	// Creates a bitmap font object with the supplied texture
	static BitmapFont* AddBitmapFont( Texture* texture, int charWidth, int charHeight, int point, std::string name );

	// Returns a pointer to the bitmap font object
	static BitmapFont* GetBitmapFont( std::string name );

	// Returns true if the bitmap font is in the container
	static bool HasBitmapFont( std::string name );

	// Clears out the container
	// -Deletes all objects
	static void Clear();
};  

#endif // !BITMAP_FONT_CONTAINER_H
