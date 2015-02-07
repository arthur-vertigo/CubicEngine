/***

Hud objects: Defines objects that will interact with the user

***/

#ifndef HUD_OBJECTS_H
#define HUD_OBJECTS_H

#include "DisplayObject.h"
#include "StaticObject2D.h"
#include "ModelContainer.h"
#include "DynamicMesh.h"
#include "BitmapFont.h"
#include "Metrics.h"
#include "Model.h"

// Bar Class: Graphical representation of a value
class Bar : public DisplayObject
{
private:
	// Bar components
	StaticObject2D background;
	StaticObject2D border;
	StaticObject2D inside;

	// Bar size and position
	Size2F size;
	Point2F position;
	int borderSize;

	// The maximum value the bar will represent
	double maxValue;

	// The current value the bar is showing
	double value;

	void ResizeBars();
	void SetBarPositions();

public:
	Bar();

	// Size / Position mutators
	void SetSize( float w, float h );
	void SetPosition( float x, float y );

	void SetValue( double value );

	void draw() ;
	void move() {};
	void handleEvent( SDL_Event& e ) {};
	void drawDebugInfo( double length ) {};
	void Pause() {};
	void Unpause() {};

	// Bar component tint mutators
	void SetBarTint( float r, float g, float b, float a );
	void SetBackgroundTint( float r, float g, float b, float a );
	void SetBorderTint( float r, float g, float b, float a );

	// Bar component texture mutators
	void SetBarTexture( Texture* textureId );
	void SetBackgroundTexture( Texture* textureId );
	void SetBorderTexture( Texture* textureId );

	void SetMaxValue( double value );

	void SetBorderSize( int newBorderSize );
};

// Mouse Class: Defines a mouse cursor
class MouseCursor : public DisplayObject
{
private:
	// Mouse display object
	StaticObject2D mouse;

	// Size of the cursor
	Size2F size;

public:
	MouseCursor();

	// Size / Position mutators
	void SetSize( float w, float h );
	void SetPosition( float x, float y );

	void draw();
	void move() {};
	void handleEvent( SDL_Event& e );
	void drawDebugInfo( double length ) {};
	void Pause() {};
	void Unpause() {};

	// Mouse tint / texture mutator
	void SetMouseTint( float r, float g, float b, float a );
	void SetMouseTexture( Texture* textureId );
};

// Button Class: Defines a clickable button
class Button : public DisplayObject
{
private:
	// Button display obj
	StaticObject2D button;

	// Function pointer to the mouse events
	// -Functions are required to take a btn* as the first argument
	void (*ClickEvent)(Button*);
	bool canClick;

	void (*ReleaseEvent)(Button*);
	bool canRelease;

	void (*OverEvent)(Button*);
	bool canOver;

	void (*ClickRollout)(Button*);
	bool canClickRollout;

	// Button demographcis
	Point2F position;
	Size2F size;

	// Mouse state flags
	bool mouseDown;
	bool mouseOver;

public:
	Button();

	// The value field that can be modified by the button
	int* value;
	void SetValue( int* value );

	// The bool field that can be modified by the button
	bool* flag;
	void SetFlag( bool* flag );

	// Size / Position mutators
	void SetSize( float w, float h );
	void SetHitSize( float w, float h );
	void SetButtonSize( float w, float h );
	void SetPosition( float x, float y );
	Size2F GetSize();

	// Sets the button click event function pointer
	void SetButtonClickEvent( void (*NewClickEvent)(Button*) );

	// Sets the button release event function pointer
	void SetButtonReleaseEvent( void (*NewReleaseEvent)(Button*) );

	// Sets the button over event function pointer
	void SetButtonOverEvent( void (*NewOverEvent)(Button*) );

	// Sets the button click-rollout event function pointer
	void SetButtonClickRolloutEvent( void (*NewClickRolloutEvent)(Button*) );

	void draw();
	void move() {};
	void handleEvent( SDL_Event& e );
	void drawDebugInfo( double length ) {};
	void Pause() {};
	void Unpause() {};

	// Button tint / texture mutators
	void SetButtonTint( float r, float g, float b, float a );
	void SetButtonTexture( Texture* textureId );
};

// Text Box: Defines a textbox
class TextBox : public DisplayObject
{
private:
	// Font to be used in the textbox
	BitmapFont* font;

	// Content
	std::string text;

	// Position
	Point2F position;

	// Justification of the text
	int justification;

public:
	TextBox();
	void SetPosition( float x, float y );
	void SetText( std::string text );
	void SetFont( BitmapFont* font );
	void SetDisplayFormat( int justification );

	void draw();
	void move() {};
	void handleEvent( SDL_Event& e ) {};
	void drawDebugInfo( double length ) {};
	void Pause() {};
	void Unpause() {};
};

// Icon class: Simple 2D display object
class Icon : public DisplayObject
{
	// Font to be used in the textbox
	StaticObject2D icon;

public:
	Icon();
	void SetIconPosition( float x, float y );
	void SetIconSize( float w, float h );
	void SetIconTexture( Texture* texture );
	void SetIconTint( float r, float g, float b, float a );

	void draw();
	void move() {};
	void handleEvent( SDL_Event& e ) {};
	void drawDebugInfo( double length ) {};
	void Pause() {};
	void Unpause() {};
};

#endif // !HUD_OBJECTS_H
