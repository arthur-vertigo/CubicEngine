#include "BitmapFontContainer.h"

std::unordered_map<std::string,BitmapFont*> BitmapFontContainer::BitmapFonts;

BitmapFont* BitmapFontContainer::AddBitmapFont( Texture* texture, int charWidth, int charHeight, int point, std::string name )
{
	if( BitmapFonts.find( name ) != BitmapFonts.end() )
		return BitmapFonts[name];

	BitmapFont* newFont = new BitmapFont;

	newFont->SetBitmap( texture, point );
	newFont->SetCharSize( static_cast<int>( charWidth * ( point / 10.0 ) ), static_cast<int>( charHeight * ( point / 10.0 ) ) );

	BitmapFonts[name] = newFont;

	return BitmapFonts[name];
}

BitmapFont* BitmapFontContainer::GetBitmapFont( std::string name )
{
	if( BitmapFonts.find( name ) == BitmapFonts.end() )
		return NULL;

	return BitmapFonts[name];
}

bool BitmapFontContainer::HasBitmapFont( std::string name )
{
	if( BitmapFonts.find( name ) == BitmapFonts.end() )
		return false;
	return true;
}

void BitmapFontContainer::Clear()
{
	for( auto iter = BitmapFonts.begin(); iter != BitmapFonts.end(); iter++ )
	{
		delete iter->second;
	}
	BitmapFonts.clear();
}