#include "HudObjects.h"


//
//  Bar Class
//

Bar::Bar()
{
	size.w = 100.f;
	size.h = 50.f;
	position.x = 100.f;
	position.y = 100.f;
	borderSize = 5;

	maxValue = 100.0;
	value = maxValue;

	background.setTint( 0.0, 0.0, 0.0, 1.0 );
	inside.setTint( 1.0, 0.0, 0.0, 1.0 );
	border.setTint( 0.5, 0.5, 0.5, 1.0 );

	ResizeBars();
	SetBarPositions();
}

void Bar::SetMaxValue( double value )
{
	maxValue = value;
}

void Bar::ResizeBars()
{
	border.setSize( size.w, size.h, 0.0 );

	double barMaxWidth = size.w - ( borderSize * 2 );
	double barMaxHeight = size.h - ( borderSize * 2 );

	background.setSize( barMaxWidth, barMaxHeight, 0.0 );

	inside.setSize( barMaxWidth / maxValue * value, barMaxHeight, 0.0 );
}

void Bar::SetBorderSize( int newBorderSize )
{
	if( newBorderSize >= 1 )
		borderSize = newBorderSize;
	else
		borderSize = 1;
}

void Bar::SetSize( float w, float h )
{
	if( w < 3 )
		w = 3;

	if( h < 3 )
		h = 3;

	size.w = w;
	size.h = h;

	ResizeBars();
}

void Bar::SetBarPositions()
{
	background.setPosition( position.x, position.y, 0.0 );
	border.setPosition( position.x, position.y, 0.0 );

	double maxBarSize = size.w - ( borderSize * 2 );
	double emptySpace = maxBarSize - ( maxBarSize * ( value / maxValue ) );
	double insideX = position.x - emptySpace / 2.0 ;

	inside.setPosition( insideX, position.y, 0.0 );
}

void Bar::SetPosition( float x, float y )
{
	position.x = x;
	position.y = y;

	SetBarPositions();
}

void Bar::SetValue( double value )
{
	if( value < 0 )
		value = 0;

	if( value > maxValue )
		value = maxValue;

	this->value = value;

	ResizeBars();
	SetBarPositions();
}

void Bar::draw()
{
	if( !this->visible )
		return;

	border.draw();
	background.draw();

	if( value != 0 )
		inside.draw();
}

void Bar::SetBarTint( float r, float g, float b, float a )
{
	inside.setTint( r, g, b, a );
}

void Bar::SetBackgroundTint( float r, float g, float b, float a )
{
	background.setTint( r, g, b, a );
}

void Bar::SetBorderTint( float r, float g, float b, float a )
{
	border.setTint( r, g, b, a );
}

void Bar::SetBarTexture( Texture* textureId )
{
	inside.SetTexture( textureId->textureID );
}

void Bar::SetBackgroundTexture( Texture* textureId )
{
	background.SetTexture( textureId->textureID );
}

void Bar::SetBorderTexture( Texture* textureId )
{
	border.SetTexture( textureId->textureID );
}

// Mouse Cursor Class

MouseCursor::MouseCursor()
{
	mouse.setSize( 5.0, 15.0, 0.0 );
	size.w = 5.0;
	size.h = 15.0;
}

void MouseCursor::SetSize( float w, float h )
{
	mouse.setSize( w, h, 0.0 );
	size.w = w;
	size.h = h;
}

void MouseCursor::SetPosition( float x, float y )
{
	// Offset to place mouse TL corner on mouse pointer
	mouse.setPosition( x + size.w / 2.0, y + size.h / 2.0 , 0.0 );
}

void MouseCursor::draw()
{
	if( !this->visible )
		return;

	mouse.draw();
}

void MouseCursor::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_MOUSEMOTION )
	{
		SetPosition( e.motion.x, e.motion.y );
	}
}

void MouseCursor::SetMouseTint( float r, float g, float b, float a )
{
	mouse.setTint( r, g, b, a );
}

void MouseCursor::SetMouseTexture( Texture* textureId )
{
	mouse.SetTexture( textureId->textureID );
}

//
// Button Class
//

Button::Button()
{
	button.setSize( 64.0, 64.0, 0.0 );
	size.w = 64.0;
	size.h = 64.0;

	ClickEvent = NULL;
	ReleaseEvent = NULL;
	OverEvent = NULL;
	ClickRollout = NULL;

	canClick = true;
	canOver = true;
	canRelease = true;
	canClickRollout = true;

	value = NULL;
	flag = NULL;

	mouseDown = false;
	mouseOver = false;
}

void Button::SetValue( int* value )
{
	this->value = value;
}

void Button::SetFlag( bool* flag )
{
	this->flag = flag;
}

void Button::SetSize( float w, float h )
{
	size.w = w;
	size.h = h;
	SetButtonSize( w, h );
}

void Button::SetHitSize( float w, float h )
{
	size.w = w;
	size.h = h;
}

void Button::SetButtonSize( float w, float h )
{
	button.setSize( w, h, 0.0 );
}

void Button::SetPosition( float x, float y )
{
	position.x = x;
	position.y = y;
	button.setPosition( x, y, 0.0 );
}
Size2F Button::GetSize()
{
	return size;
}

void Button::SetButtonClickEvent( void (*NewClickEvent)(Button*) )
{
	ClickEvent = NewClickEvent;
}


void Button::SetButtonReleaseEvent( void (*NewReleaseEvent)(Button*) )
{
	ReleaseEvent = NewReleaseEvent;
}

void Button::SetButtonOverEvent( void (*NewOverEvent)(Button*) )
{
	OverEvent = NewOverEvent;
}


void Button::SetButtonClickRolloutEvent( void (*NewClickRolloutEvent)(Button*) )
{
	ClickRollout = NewClickRolloutEvent;
}

void Button::draw()
{
	if( !this->visible )
		return;

		button.draw();
}

void Button::handleEvent( SDL_Event& e )
{
	if( !visible )
		return;

	if( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
	{
		int left = static_cast<int>( position.x - ( size.w / 2.0 ) );
		int right = static_cast<int>( position.x + ( size.w / 2.0 ) );
		int top = static_cast<int>( position.y + ( size.h / 2.0 ) );
		int bottom = static_cast<int>( position.y - ( size.h / 2.0 ) );

		// If true then were on the button
		if( ( e.motion.x > left && e.motion.x < right ) && ( e.motion.y > bottom && e.motion.y < top  ) )
		{
			if( !mouseDown ) // If structure to prevent click rollover mouse down events
			{
				mouseOver = true;
				canClickRollout = true; // Set the opposite event flags to true, this only allows one execution of the event functions per event 
			}

			if( OverEvent != NULL && canOver )
			{
				canOver = false;
				OverEvent( this );
			}
		}
		else
		{
			mouseOver = false;
			canOver = true;
			if( ClickRollout != NULL && mouseDown == true && canClickRollout )
			{
				canClickRollout = false;
				ClickRollout( this );
			}
		}
	}

	if( e.type == SDL_MOUSEBUTTONDOWN )
	{
		if( e.button.button == SDL_BUTTON_LEFT )
		{
			mouseDown = true;
			canRelease = true;
			if( ClickEvent != NULL && mouseOver && canClick )
			{
				canClick = false;
				ClickEvent( this );
			}
		}
	}


	if( e.type == SDL_MOUSEBUTTONUP )
	{
		if( e.button.button == SDL_BUTTON_LEFT )
		{
			mouseDown = false;
			canClick = true;
			if( ReleaseEvent != NULL && mouseOver && canRelease )
			{
				canRelease = false;
				ReleaseEvent( this );
			}

		}
	}

}

void Button::SetButtonTint( float r, float g, float b, float a )
{
	button.setTint( r, g, b, a );
}

void Button::SetButtonTexture( Texture* textureId )
{
	button.SetTexture( textureId->textureID );
}

// Textbox class
// Text Box: Defines a textbox

TextBox::TextBox()
{
	font = NULL;
	text = "";
	position.x = 0;
	position.y = 0;
	justification = BitmapFont::CENTER;
}

void TextBox::SetPosition( float x, float y )
{
	position.x = x;
	position.y = y;
}

void TextBox::SetText( std::string text )
{
	this->text = text;
}

void TextBox::SetFont( BitmapFont* font )
{
	this->font = font;
}

void TextBox::SetDisplayFormat( int justification )
{
	this->justification = justification;
}

void TextBox::draw()
{
	if( !this->visible )
		return;

	if( font != NULL )
		font->RenderText( position.x, position.y, text, justification );
}

//
// Icon Class
//

Icon::Icon()
{
}

void Icon::SetIconPosition( float x, float y )
{
	icon.setPosition( x, y, 0.0 );
}

void Icon::SetIconSize( float w, float h )
{
	icon.setSize( w, h, 0.0 );
}

void Icon::SetIconTexture( Texture* texture )
{
	icon.SetTexture( texture->textureID );
	icon.setSize( texture->textureSize.w, texture->textureSize.h, 0.0 );
}

void Icon::SetIconTint( float r, float g, float b, float a )
{
	icon.setTint( r, g, b, a );
}

void Icon::draw()
{
	if( !this->visible )
		return;

		icon.draw();
}
