/***

Bitmap Font: BITMAP FONT!

***/

#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_opengl.h"

#include "Metrics.h"
#include "StaticObject2D.h"
#include "DisplayScreen.h"

class BitmapFont
{
private:
	// True if the font has been set
	bool fontSet;

	// The font bitmap
	StaticObject2D font;

	// Size of the bitmap font ***REFACTOR***
	Size2I bitmapSize;

	// The size of the char
	Size2I charSize;

	// Padding between chars
	Size2I padding;

	// The starting ascii character on the sprite sheet
	char startingAsciiCharacter;

public:
	BitmapFont();

	// Rendering justifications
	enum justifications
	{
		CENTER,
		LEFT,
		RIGHT
	};

	// Sets the butmap of the font
	void SetBitmap( Texture* textureID, int point );

	// Sets the size of the chars
	void SetCharSize( int w, int h );

	// Sets the padding size
	void SetPaddingSize( int w, int h );

	// Sets the first char on the sprite sheet
	void SetFirstChar( char c );

	// Renders the text at x, y
	void RenderText( double x, double y, std::string text, int justification = 0 );
};

#endif // !BITMAP_FONT_H
