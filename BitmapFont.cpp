#include "BitmapFont.h"

/*
class BitmapFont
{
private:
	GLuint fontBitmap;

	Size2I charSize;
	Size2I padding;

public:
	BitmapFont();

	void SetBitmap( GLuint textureID );
	void SetCharSize( int w, int h );
	void SetPaddingSize( int w, int h );

	void RenderText( double x, double y, std::string text );
};

*/

BitmapFont::BitmapFont()
{
	fontSet = false;
	charSize.w = 8;
	charSize.h = 8;
	startingAsciiCharacter = ' ';
	bitmapSize.w = 0;
	bitmapSize.h = 0;
}

void BitmapFont::SetBitmap( Texture* texture, int point )
{
	bitmapSize.w = static_cast<int>( texture->textureSize.w * ( point / 10.0 ) );
	bitmapSize.h = static_cast<int>( texture->textureSize.h * ( point / 10.0 ) );

	font.SetTexture( texture->textureID );
	font.setSize( bitmapSize.w, bitmapSize.h, 0.0 );
	fontSet = true;
}

void BitmapFont::SetCharSize( int w, int h )
{
	if( w < 1 )
		w = 1;

	if( h < 1 )
		h = 1;

	charSize.w = w;
	charSize.h = h;
}

void BitmapFont::SetPaddingSize( int w, int h )
{
	if( w < 0 )
		w = 0;

	if( h < 0 )
		h = 0;

	padding.w = w;
	padding.h = h;
}

void BitmapFont::SetFirstChar( char c )
{
	startingAsciiCharacter = c;
}

void BitmapFont::RenderText( double x, double y, std::string text, int justification )
{
	if( !fontSet )
		return;

	int size = text.size();
	Size2I screenSize = DisplayScreen::GetScreenSize();
	int totalLength = size * charSize.w;

	StringFunctions::ToUpper( text );
	
	glEnable( GL_SCISSOR_TEST );

	for( int i = 0; i < size; i++ )
	{
		if( text[i] - startingAsciiCharacter < 0 )
			continue;

		switch( justification )
		{
			case LEFT:
				// Set font sheet into position
				font.setPosition( x + ( charSize.w * i )  - ( (text[i] - startingAsciiCharacter ) * charSize.w ) + ( bitmapSize.w / 2.0 ), y , 0.0 );

				// only draw the character at current position
				glScissor( static_cast<int>( x + ( charSize.w * i ) ), static_cast<int>( screenSize.h - ( y + charSize.h / 2.0 ) ) , charSize.w, charSize.h );
			break;

			default:
			case CENTER:
				// Set font sheet into position
				font.setPosition( x + ( charSize.w * i )  - ( (text[i] - startingAsciiCharacter ) * charSize.w ) + ( bitmapSize.w / 2.0 ) - ( totalLength / 2.0 ), y , 0.0 );

				// only draw the character at current position
				glScissor( static_cast<int>(  x + ( charSize.w * i ) - ( totalLength / 2.0 ) ) , static_cast<int>(  screenSize.h - ( y + charSize.h / 2.0 ) ) , charSize.w, charSize.h );
			break;

		}
		font.draw();
	}

	glDisable( GL_SCISSOR_TEST );

}