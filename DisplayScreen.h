/***

Display Screen: Controls opengl rendering parameters

***/

#ifndef DISPLAY_SCREEN_H
#define DISPLAY_SCREEN_H

#include <ctime>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#include "SDL_opengl.h"

#include "Metrics.h"

class DisplayScreen
{
private:

	// The size of the screen
	static Size2I ScreenSize;

	// The FOV for 3D rendering
	static int FOV;

	// The near clip range for 3D rendering
	static double nearClip;

	// The far clip range for 3D rendering
	static double farClip;

	// Flags for various opengl states
	static bool depthTest;
	static bool wireframe;

	// If the display screen is fullscreen
	static bool fullscreen;

	// Sets the internal SDL screen to the screensize, etc
	static void SetSDLScreen();

	// True if the screen has been enables at least once
	static bool hasInit;

public:

	// Sets the starting opengl, sdl parameters
	static void init();

	// Rendering modes supported
	enum RENDER_MODES
	{
		SOLID,
		WIREFRAME
	};

	// Screen size mutators / assessors
	static void SetScreenSize( int w, int h );
	static Size2I GetScreenSize();

	static void SetFullscreen( bool flag );

	// Sets the 2D rendering opengl parameters
	// - Any object rendered after calling this function is assumed to be in screen space
	static void Set2DRendering();

	// Sets the 3D rendering opengl parameters
	// - Any object rendered after calling this function is assumed to be in world space
	static void Set3DRendering();

	// Sets the rendering mode ( wireframe, solid )
	static void SetRenderMode( int renderMode );

	static void SwapFrameBuffers();
	static void ClearScreen();
};

#endif // !DISPLAY_SCREEN_H
