#include "DisplayScreen.h"

Size2I DisplayScreen::ScreenSize;
int DisplayScreen::FOV = 90;
double DisplayScreen::nearClip = 0.01;
double DisplayScreen::farClip = 50000.0;

bool DisplayScreen::depthTest = true;
bool DisplayScreen::fullscreen = false;
bool DisplayScreen::hasInit = false;
bool DisplayScreen::wireframe = false;

void DisplayScreen::SetSDLScreen()
{
	if( fullscreen )
		SDL_SetVideoMode( ScreenSize.w, ScreenSize.h, 32, SDL_OPENGL | SDL_FULLSCREEN );
	else
		SDL_SetVideoMode( ScreenSize.w, ScreenSize.h, 32, SDL_OPENGL );
}

void DisplayScreen::ClearScreen()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void DisplayScreen::SwapFrameBuffers()
{
	SDL_GL_SwapBuffers();
}

void DisplayScreen::Set3DRendering()
{
	if( !glIsEnabled( GL_DEPTH_TEST ) )
		glEnable( GL_DEPTH_TEST );

	if( wireframe )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( FOV, (ScreenSize.w/ScreenSize.h), nearClip, farClip );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void DisplayScreen::Set2DRendering()
{
	if( glIsEnabled( GL_DEPTH_TEST ) )
		glDisable( GL_DEPTH_TEST );

	if( wireframe ) // Dont go into wireframe mode for 2d graphics
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, ScreenSize.w, ScreenSize.h, 0, -1000, 1000 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

}

Size2I DisplayScreen::GetScreenSize()
{
	return ScreenSize;
}

void DisplayScreen::SetScreenSize( int w, int h )
{

	if( w < 0 )
		w = 1;

	if( h < 0 )
		h = 1;

	ScreenSize.w = w;
	ScreenSize.h = h;

	if( hasInit )
		SetSDLScreen();
}

void DisplayScreen::SetFullscreen( bool flag )
{

	fullscreen = flag;

	if( hasInit )
		SetSDLScreen();
}

void DisplayScreen::init()
{

	srand( static_cast<int>( time( NULL ) ) );

	SDL_Init(SDL_INIT_EVERYTHING);

	SetSDLScreen();

	SDL_WM_SetCaption( "Cubic Engine", NULL);
	SDL_WM_GrabInput( SDL_GRAB_ON );
	SDL_ShowCursor(0); 

	// Blending/Textures
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	// Lighting
	glEnable(GL_NORMALIZE);
	glShadeModel( GL_SMOOTH );
	glEnable( GL_CULL_FACE );
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable( GL_COLOR_MATERIAL );


	static GLfloat spec[] = { 0.8f, 0.8f, 0.8f , 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	static GLfloat diff[] = { 1.0f, 1.0f, 1.0f , 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

	static GLfloat amb[] = { 0.1f, 0.1f, 0.1f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );

	// AA ?
	//glEnable( GL_POLYGON_SMOOTH );
	//glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	
	//Fog
	glEnable( GL_FOG );
	glFogi (GL_FOG_MODE, GL_EXP);
	glHint (GL_FOG_HINT, GL_NICEST);
	
	// Enable depth
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glClearColor( 0.0, 0.0, 0.0, 0 );

	// SDL Attributes
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	glLineWidth( 1.0f );

	hasInit = true;
}

void DisplayScreen::SetRenderMode( int renderMode )
{
	switch( renderMode )
	{
		case WIREFRAME:
			wireframe = true;
		break;

		case SOLID:
		default:
			wireframe = false;
		break;
	}
}