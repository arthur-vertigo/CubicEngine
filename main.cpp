/***

Cubic Engine Test Application
Programmer: Arthur Wuterich
Data Start: 01/07/2013
Current Date: 01/27/2013

***/

#include <vector>
#include <sstream>
#include <ctime>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_Mixer.h"
#define GL_GLEXT_PROTOTYPES
#include "SDL_opengl.h"

#include "BitmapFontContainer.h"
#include "GLTextureContainer.h"
#include "AnimationContainer.h"
#include "MaterialContainer.h"
#include "TerrainGenerator.h"
#include "ObjectController.h"
#include "AnimatedObject.h"
#include "ModelContainer.h"
#include "StaticObject2D.h"
#include "DisplayScreen.h"
#include "StaticObject.h"
#include "DynamicMesh.h"
#include "Environment.h"
#include "WorldObject.h"
#include "HudObjects.h"
#include "Utilities.h"
#include "Vector.h"
#include "Camera.h"
#include "Model.h"
#include "Math.h"

// Game specific classes
#include "GameClasses.h"

// Efficency Variables

// System Variables

const double RENDER_DISTANCE = 900.0;

const bool FULLSCREEN = false;
//const bool FULLSCREEN = false;
//const int SCREEN_WIDTH = 800; // Low Res
//const int SCREEN_HEIGHT = 600;
//const int SCREEN_WIDTH = 1366; // Laptop
//const int SCREEN_HEIGHT = 768;
const int SCREEN_WIDTH = 1440; // 3/4 ar
const int SCREEN_HEIGHT = 900;
//const int SCREEN_WIDTH = 1920; // full
//const int SCREEN_HEIGHT = 1200;
const bool DISABLE_ENV_STATES = false;
const bool WORLD_READ_ONLY = false;

// If true, will delete all container classes, and object controllers dynamically
// generated data
const bool CLEANUP_MEMORY_ON_EXIT = false;
const bool CLEANUP_VBO_IBO_ON_EXIT = false;

// Functions altering meshes
namespace MeshFunctions
{

	bool ChangeFunction1( Model* model )
	{
		const int GRASS_CHANGE_VALUE = -100;
		const std::string GRASS_TOP_TEXTURE = "Mat.Grass";
		const std::string GRASS_MIDDLE_TEXTURE = "Mat.Dirt_Grass";
		const std::string GRASS_UPPER_TEXTURE = "Mat.Dirt";
		const std::string GRASS_LOWER_TEXTURE = "Mat.Rock";

		bool returnBool = false;
	
		for( int i = 0; i < model->amountOfFaces; i++ )
		{
			if(  model->faces[i].verts[0]->position.y + 
					model->faces[i].verts[1]->position.y + 
					model->faces[i].verts[2]->position.y > GRASS_CHANGE_VALUE &&
					model->faces[i].material != GRASS_TOP_TEXTURE )
			{
				model->faces[i].material = GRASS_TOP_TEXTURE;
				returnBool = true;
			}

			if(  model->faces[i].verts[0]->position.y + 
					model->faces[i].verts[1]->position.y + 
					model->faces[i].verts[2]->position.y < GRASS_CHANGE_VALUE &&
					model->faces[i].material != GRASS_MIDDLE_TEXTURE )
			{
				model->faces[i].material = GRASS_MIDDLE_TEXTURE;
				returnBool = true;
			}

			if(  model->faces[i].verts[0]->position.y +
					model->faces[i].verts[1]->position.y +
					model->faces[i].verts[2]->position.y < GRASS_CHANGE_VALUE*2 &&
					model->faces[i].material != GRASS_UPPER_TEXTURE )
			{
				model->faces[i].material = GRASS_UPPER_TEXTURE;
				returnBool = true;
			}

			if(  model->faces[i].verts[0]->position.y +
					model->faces[i].verts[1]->position.y +
					model->faces[i].verts[2]->position.y < GRASS_CHANGE_VALUE*3 &&
					model->faces[i].material != GRASS_LOWER_TEXTURE )
			{
				model->faces[i].material = GRASS_LOWER_TEXTURE;
				returnBool = true;
			}
		}
	
		return returnBool;
	}
}

// Functions to control button behavior
namespace ButtonFunctions
{
	// Quit button event
	void SetToTrue( Button* btn )
	{
		if( btn->flag != NULL )
			*btn->flag = true;
	}

	void SetToFalse( Button* btn )
	{
		if( btn->flag != NULL )
			*btn->flag = false;
	}

	void Shrink( Button* btn )
	{
		Size2F size = btn->GetSize();
		btn->SetButtonSize( size.w - 10, size.h - 10 );
	}

	void Expand( Button* btn )
	{
		Size2F size = btn->GetSize(); // This returns the hit size of the button; which is already at its normal size
		btn->SetButtonSize( size.w, size.h );
	}
}

// Removes objects / resources before application closes
void CleanUp()
{
	if ( CLEANUP_MEMORY_ON_EXIT )
	{
		GLTextureContainer::clear();
		MaterialContainer::clear();
		ModelContainer::clear();
		ObjectController::clearAllControllers();
		TerrainGenerator::ClearAllTerrainGenerators();
	}

	if( CLEANUP_VBO_IBO_ON_EXIT )
	{
		ModelContainer::ClearVBO_IBOs();
	}

	SDL_Quit();    
}

// Sets user defined materials
void SetMaterials()
{

	MaterialContainer::NewMaterial( "Mat.Grass" );
	//MaterialContainer::SetDiffuse( Color4F( 0.0, 1.0, 0.0, 1.0 ) );
	MaterialContainer::SetSpecularity( Color4F( 0.0, 0.0, 0.0, 0.0 ) );
	MaterialContainer::SetSpecularityExponent( 0 );
	MaterialContainer::SetTexture( GLTextureContainer::addTexture( "image/grass.png", "grass" ) );

	MaterialContainer::NewMaterial( "Mat.Dirt_Grass" );
	MaterialContainer::SetDiffuse( Color4F( 0.5, 1.0, 0.5, 1.0 ) );
	MaterialContainer::SetSpecularityExponent( 10 );
	MaterialContainer::SetTexture( GLTextureContainer::addTexture( "image/dirt_grass.png", "dirt_grass" ) );

	MaterialContainer::NewMaterial( "Mat.Dirt" );
	MaterialContainer::SetTexture( GLTextureContainer::addTexture( "image/dirt.png", "dirt" ) );

	MaterialContainer::NewMaterial( "Mat.Rock" );
	MaterialContainer::SetTexture( GLTextureContainer::addTexture( "image/rock.png", "rock" ) );

	MaterialContainer::NewMaterial( "Mat.Water" );
	MaterialContainer::SetDiffuse( Color4F(1.0, 1.0, 1.0, .75) );
	MaterialContainer::SetTexture( GLTextureContainer::addTexture( "image/water.png", "water" ) );

}

void SetupHud( ObjectController& ctr )
{
}

// Loads the models for the application
void LoadModels()
{
	ModelContainer::addModel( "obj/BatmanArmoured.obj", "Batman", true );
	//ModelContainer::addModel( "obj/manyObjs.obj", "ManyObjs", true );
	//ModelContainer::addModel( "obj/Harley.obj", "Harley", true );
	ModelContainer::addModel( "obj/BatmanArmoured.obj", "Batman", true );
	ModelContainer::addModel( "obj/cow.obj", "cow", true );
	ModelContainer::addModel( "obj/orb.obj", "orb", true );
	//ModelContainer::addModel( "obj/PlayerBody.obj", "PlayerBody", true, false );
	//ModelContainer::addModel( "obj/testcube.obj", "cube", true );
	//ModelContainer::addModel( "obj/teapot.obj", "teapot", true );
}

// Sets the worlds environment states
void SetWorldEnvironmentStates( Environment& env )
{
	const double STATE_MULTIPLIER = 30.0;
	const double STATE_DURATION = 60.0 * STATE_MULTIPLIER;
	const double TRANSITION_DURATION = 30.0 * STATE_MULTIPLIER;
	const double LIGHT_DISTANCE = RENDER_DISTANCE;

	// Day
	env.NewState();
	env.SetSkyColor( Color3F( 0.f, 0.54f, 0.7f ) );
	env.SetFogColor( Color3F( 0.2f, 0.2f, 0.2f ) );
	env.SetFogExponent( 0.01 );
	env.SetLightColor( Color3F( 1.0, 1.0, 1.0) );
	env.SetLightPosition( Point3D( LIGHT_DISTANCE, LIGHT_DISTANCE, 0.0 ) );
	env.SetDuration( STATE_DURATION ); // Duration

	env.NewState();
	env.SetLightPosition( Point3D( -LIGHT_DISTANCE, LIGHT_DISTANCE, 0.0 ) );
	env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change

	if( !DISABLE_ENV_STATES )
	{

	// Dusk
		env.NewState();
		env.SetSkyColor( Color3F( 0.5f, 0.22f, 0.32f ) );
		env.SetFogColor( Color3F( 0.1f, 0.1f, 0.1f ) );
		env.SetFogExponent( 0.02 );
		env.SetLightColor( Color3F( 1.0, 0.8, 0.8) );
		env.SetLightPosition( Point3D( -LIGHT_DISTANCE, 0.0, 0.0 ) );
		env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change 1

		env.NewState();
		env.SetLightPosition( Point3D( LIGHT_DISTANCE, -LIGHT_DISTANCE, 0.0 ) );
		env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change 2

		env.NewState();
		env.SetLightPosition( Point3D( LIGHT_DISTANCE, 0.0, 0.0 ) );
		env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change 3

	// Night

		env.NewState();
		env.SetSkyColor( Color3F( 0.f, 0.06f, 0.11f ) );
		env.SetFogColor( Color3F( 0.0f, 0.0f, 0.0f ) );
		env.SetFogExponent( 0.08 );
		env.SetLightColor( Color3F( 0.01, 0.01, 0.01 ) );
		env.SetLightPosition( Point3D( LIGHT_DISTANCE, LIGHT_DISTANCE, 0.0 ) );
		env.SetDuration( STATE_DURATION ); // Duration

		env.NewState();
		env.SetLightPosition( Point3D( -LIGHT_DISTANCE, LIGHT_DISTANCE, 0.0 ) );
		env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change

	// Morning

		env.NewState();
		env.SetSkyColor( Color3F( 0.f, 0.34f, 0.9f ) );
		env.SetFogColor( Color3F( 0.1f, 0.1f, 0.1f ) );
		env.SetFogExponent( 0.005 );
		env.SetLightColor( Color3F( 0.8, 0.8, 1.0) );
		env.SetLightPosition( Point3D( -LIGHT_DISTANCE, 0.0, 0.0 ) );
		env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change 1

		env.NewState();
		env.SetLightPosition( Point3D( LIGHT_DISTANCE, -LIGHT_DISTANCE, 0.0 ) );
		env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change 2

		env.NewState();
		env.SetLightPosition( Point3D( LIGHT_DISTANCE, 0.0, 0.0 ) );
		env.SetDuration( TRANSITION_DURATION / 2.0 ); // Change 3
	} 

	env.SetAutomaticStateChange( true );
	env.SetState( 0 );

}

// Saves the current world
void SaveWorld( TerrainGenerator tg, std::string name )
{
	tg.SaveWorld( "export/" + name+".wrld" );
	ModelContainer::ExportModels( "export/", "chunk" );
}

// Finds the height of a point in the current chunk's terrain ***REFACTOR***
double GetChunkPointHeight( Model* chunk, Point3D position, Point3D chunkPosition )
{
	if( chunk == NULL )
		return 0.0;

	position.x -= chunkPosition.x;
	position.z -= chunkPosition.z;

	double highestPoint = -10000.0;
	double newPoint = 0.0;
	for( int i = 0; i < chunk->amountOfFaces; i++ )
	{
		if( Math::pointInTriangleSign( position, chunk->faces[i].verts[0]->position, chunk->faces[i].verts[1]->position, chunk->faces[i].verts[2]->position ) )
		{
			newPoint = Math::calcY( chunk->faces[i].verts[0]->position, chunk->faces[i].verts[1]->position, chunk->faces[i].verts[2]->position, position.x, position.z );

			if( newPoint > highestPoint )
				highestPoint = newPoint;
		}
	}

	return highestPoint;
}

// Creates animations / loads animation sequences
void SetAnimations()
{
	// Animations
	AnimationContainer::NewAnimation( "cow_jump" );
	for( double i = 0.0; i < 3.14; i += 0.1 )
	{
		AnimationContainer::AddFrameToAnimation(	ModelContainer::getModel( "Batman" ), 
													&Size3D(0.5,0.5,0.5), 
													&Rotation3D( i, i*5.0, i ), 
													&Point3D( 0.0, sin(i), 0.0 ) );
	}
	AnimationContainer::DoubleFramesAndReverseTail();
	
	AnimationContainer::LoadBlenderAnimationSequence( "obj/Box/Box_000001.obj", "Sheep" );
}

// Sets the bitmap fonts for the application
void SetFonts()
{
	BitmapFontContainer::AddBitmapFont( GLTextureContainer::addTexture( "image/font.png", "font1" ), 8, 8, 10, "DefaultFont" );
}

int main(int argc, char* args[])
{
	// Application variables
	bool quit = false;
	int currentScene = 0;
	double BrushSize = 1.0;
	SDL_Event event;
	Timer dynamicMeshTimer;
	FpsMeter fps;
	bool mouseButton[2] = { false, false };
    
	// Display Screen
	DisplayScreen::SetFullscreen( FULLSCREEN );
	DisplayScreen::SetScreenSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	DisplayScreen::init(); // Starts SDL & OpenGL

	// Rendering
	//  -Sets the default distance that objects in object controllers will draw at
	ObjectController::SetRenderDistance( RENDER_DISTANCE );

	// Sets custom materials
	SetMaterials();
	
	// Object controllers
	// mainController controls all of the 3d elements
	// hudController controls all of the 2d hud elements
	ObjectController mainController;
	ObjectController hudController;

	// Loads models used in application
	LoadModels();

	// Sets the bitmap fonts for application
	SetFonts();

	// Cameras
	Camera mainCamera;

	// Creates animations
	SetAnimations();

	// Generators
	Environment worldEnvironment;

	// Sets the environment states
	SetWorldEnvironmentStates( worldEnvironment );
	worldEnvironment.SetControllableLight( GL_LIGHT0 );

	TerrainGenerator worldTerrain;
	worldTerrain.LoadWorld( "export/world1.wrld" );
	worldTerrain.SetChunkSize( 300.0 );
	worldTerrain.SetChunkDetail( 40 );
	mainController.addObject( &worldTerrain );

	// Game Assets
	Player mainPlayer;
	mainPlayer.SetPosition( worldTerrain.GetPlayerPosition() );
	mainController.addObject( &mainPlayer );

	// 3D Assets
	
	for( int i = 0; i < 100; i++ )
	{
		AnimatedObject* randomAO = new AnimatedObject;

		if( Math::randomDouble( 0.0, 100.0 ) > 50.0 )
			randomAO->SetAnimation( AnimationContainer::GetAnimation( "batman_twist" ) );
		else if( Math::randomDouble( 0.0, 100.0 ) > 50.0 )
			randomAO->SetAnimation( AnimationContainer::GetAnimation( "cow_jump" ) );
		else
			randomAO->SetAnimation( AnimationContainer::GetAnimation( "Sheep" ) );

		randomAO->SetLooping( true );
		randomAO->SetFrameRange( 1 );
		randomAO->GotoRandomFrame();
		randomAO->SetFPS( 60 );
		randomAO->setTint( Math::RandomFloat( -1.0, 1.0 ),Math::RandomFloat( -1.0, 1.0 ),Math::RandomFloat( -1.0, 1.0 ), 1.0 );
		randomAO->setRotation( 0.0, Math::randomDouble( 0.0, 360.0 ), 0.0 );
		randomAO->setPosition( Math::randomDouble(-300.0, 300.0 ),1.0,Math::randomDouble( -300.0, 300.0 ));
		randomAO->setSize( 5.0, 5.0, 5.0 );
		mainController.addObject( randomAO );
	}

	StaticObject* ORB = new StaticObject;
	ORB->setModel( ModelContainer::getModel( "orb" ) );
	ORB->type = "so_noOctree";
	ORB->setSize( 200.0, 200.0, 200.0 );
	ORB->setTint( 1.0, 1.0, 0.0, 1.0 );
	ORB->hasFog = false;
	ORB->hasLighting = false;
	mainController.addObject( ORB );

	/*
	StaticObject* PlayerBody = new StaticObject;
	PlayerBody->setModel( ModelContainer::getModel( "PlayerBody" ) );
	PlayerBody->setSize( 0.5, 0.5, 0.5 );
	mainController.addObject( PlayerBody );
	*/

	// 2D Assets
	Icon* shadeScreen = new Icon;
	shadeScreen->SetIconSize( static_cast<float>( SCREEN_WIDTH ), static_cast<float>( SCREEN_HEIGHT ) );
	shadeScreen->SetIconPosition( static_cast<float>( SCREEN_WIDTH / 2.0 ), static_cast<float>( SCREEN_HEIGHT / 2.0 ) );
	shadeScreen->visible = false;
	shadeScreen->SetIconTint( 0.0, 0.0, 0.0, 0.5 );
	hudController.addObject( shadeScreen );
	
	Bar* newBar = new Bar;
	newBar->SetPosition( SCREEN_WIDTH / 2.0, SCREEN_HEIGHT - 25.0 );
	newBar->SetSize( SCREEN_WIDTH - 50, 25.0 );
	newBar->SetMaxValue( 150 );

	newBar->SetBorderSize( 3 );
	newBar->SetBarTint( 1.0, 0.0, 0.0, 1.0 );
	newBar->SetBorderTint( 0.5, 0.5, 0.5, 1.0 );
	newBar->SetBackgroundTint( 0.2f, 0.2f, 0.2f, 1.0f );

	newBar->SetValue( 1.0 );
	newBar->visible = false;
	hudController.addObject( newBar );

	Icon* flyingIcon = new Icon;
	flyingIcon->SetIconTexture( GLTextureContainer::addTexture( "image/FlyingIcon.png", "FlyingIcon" ) );
	flyingIcon->SetIconPosition( SCREEN_WIDTH / 2.0, 38.0 );
	flyingIcon->SetIconSize( 64.0, 64.0 );
	flyingIcon->visible = false;
	hudController.addObject( flyingIcon );

	Button* quitButton = new Button;
	quitButton->SetFlag( &quit );
	quitButton->SetPosition( SCREEN_WIDTH - 16.0, 16.0 );
	quitButton->SetSize( 32.0, 32.0 );
	quitButton->SetButtonTexture( GLTextureContainer::addTexture( "image/ExitButton.png", "exitButton" ) );
	quitButton->SetButtonClickEvent( ButtonFunctions::Shrink );
	quitButton->SetButtonReleaseEvent( ButtonFunctions::SetToTrue );
	quitButton->SetButtonClickRolloutEvent( ButtonFunctions::Expand );
	quitButton->visible = false;
	hudController.addObject( quitButton );

	TextBox* fpsTextBox = new TextBox;
	fpsTextBox->SetFont( BitmapFontContainer::GetBitmapFont( "DefaultFont" )  );
	fpsTextBox->SetPosition( 2, 6 );
	fpsTextBox->SetDisplayFormat( BitmapFont::LEFT );
	hudController.addObject( fpsTextBox );

	TextBox* positionTextBox = new TextBox;
	positionTextBox->SetFont( BitmapFontContainer::GetBitmapFont( "DefaultFont" )  );
	positionTextBox->SetPosition( 2, 22 );
	positionTextBox->SetDisplayFormat( BitmapFont::LEFT );
	hudController.addObject( positionTextBox );

	TextBox* barTextBox = new TextBox;
	barTextBox->SetFont( BitmapFontContainer::GetBitmapFont( "DefaultFont" )  );
	barTextBox->SetPosition( SCREEN_WIDTH / 2.0, SCREEN_HEIGHT - 25.0 );
	barTextBox->SetText( "Height Position: 1, Brush Size: 1.1" );
	barTextBox->visible = false;
	hudController.addObject( barTextBox );

	TextBox* infoBox = new TextBox;
	infoBox->SetFont( BitmapFontContainer::GetBitmapFont( "DefaultFont" )  );
	infoBox->SetPosition( 2, SCREEN_HEIGHT - 6.0 );
	infoBox->SetText( "Press E to enter build mode" );
	infoBox->SetDisplayFormat( BitmapFont::LEFT );
	hudController.addObject( infoBox );

	TextBox* infoBox2 = new TextBox;
	infoBox2->SetFont( BitmapFontContainer::GetBitmapFont( "DefaultFont" )  );
	infoBox2->SetPosition( SCREEN_WIDTH / 8.0 * 5.0, 6.0 );
	infoBox2->SetText( "ESC: Menu, O: Solid Render Mode, P: Wireframe Render Mode" );
	hudController.addObject( infoBox2 );

	ToolBelt* newToolBelt = new ToolBelt(	4, 
								BitmapFontContainer::GetBitmapFont( "DefaultFont" ),
								GLTextureContainer::addTexture( "image/BlankToolBucket.png", "BlankToolBucket" ),
								GLTextureContainer::addTexture( "image/SelectedToolBucket.png", "SelectedToolBucket" ) );
	newToolBelt->SetPosition( SCREEN_WIDTH/2.0, SCREEN_HEIGHT - 70.0 );
	newToolBelt->SetBucketLable( 1, "Push" );
	newToolBelt->SetBucketLable( 2, "Sin" );
	newToolBelt->SetBucketLable( 3, "Flat" );
	newToolBelt->SetBucketLable( 4, "Hole" );
	newToolBelt->visible = false;
	hudController.addObject( newToolBelt );

	MouseCursor* mouse = new MouseCursor;
	mouse->SetMouseTexture( GLTextureContainer::addTexture( "image/cursor.png", "cursor" ) );
	mouse->SetSize( 32.0, 32.0 );
	mouse->visible = false;
	hudController.addObject( mouse );
	
	// Main Loop     
	while(quit == false)
	{
		// FPS meter update loop
		if( fps.UpdateFrame() )
		{
			std::stringstream ss;
			ss << "FPS: " << fps.GetReading(); 
			fpsTextBox->SetText( ss.str() );
		}

		// Position text box
		std::stringstream ss;
		ss << "position: (" << mainPlayer.GetPosition().x << ", " << mainPlayer.GetPosition().y << ", " << mainPlayer.GetPosition().z << " )";
		positionTextBox->SetText( ss.str() );

		DisplayScreen::ClearScreen();
		DisplayScreen::Set3DRendering();

			// Sets the main cameras pos
				mainCamera.setRotation( mainPlayer.GetRotation() );
				mainCamera.setView();
				worldEnvironment.Update( &mainPlayer.GetPosition() );

				switch( worldEnvironment.GetCurrentState() )
				{
					case 10:
						ORB->setTint( 1.0, 1.0, 0.0, 1.0 );
					break;

					case 3:
						ORB->setTint( 1.0, 1.0, 1.0, 1.0 );
					break;

				}
				ORB->setPosition( worldEnvironment.GetLightPosition() );

				// Objects
				mainController.moveObjects();
				//PlayerBody->setPosition( mainPlayer.GetPosition().x,mainPlayer.GetPosition().y-0.20,mainPlayer.GetPosition().z );
				//PlayerBody->setRotation( 0.0, -(mainPlayer.GetRotation().y + 180 ), 0.0   );
				worldTerrain.UpdatePlayerPosition( mainPlayer.GetPosition() );

				// Main Player
				if( worldTerrain.GetCurrentChunk() != NULL )
					mainPlayer.SetFloor( GetChunkPointHeight( worldTerrain.GetCurrentChunk()->getModel(), mainPlayer.GetPosition(), worldTerrain.GetCurrentChunk()->GetPosition() ) );

				// Set camera positions
				mainCamera.setPosition( mainPlayer.GetPosition() );
				ObjectController::UpdateRenderPosition( mainPlayer.GetPosition() );

				mainController.drawObjects();

		DisplayScreen::Set2DRendering();

			hudController.drawObjects();

		// End of rendering
		
		// SDL Inputs
		while(SDL_PollEvent(&event))
		{
			mainController.eventObjects( &event );
			hudController.eventObjects( &event );
  
			// Get me out of here!
			if( event.type == SDL_QUIT )
			{  
				quit = true;
			}

			if( event.type == SDL_KEYDOWN )
			{
				switch ( event.key.keysym.sym )
				{

					case SDLK_o:
						DisplayScreen::SetRenderMode( DisplayScreen::WIREFRAME );
					break;

					case SDLK_p:
						DisplayScreen::SetRenderMode( DisplayScreen::SOLID );
					break;

					case SDLK_e:
						if( mainPlayer.isFlying() )
						{
							flyingIcon->visible = true;
							newToolBelt->visible = true;
							barTextBox->visible = true;
							newBar->visible = true;
							infoBox->SetText( "LMB: action1, RMB: action2, MouseWheel: Y Position, E: exit build mode" );
						}
						else
						{
							flyingIcon->visible = false;
							newToolBelt->visible = false;
							barTextBox->visible = false;
							newBar->visible = false;
							infoBox->SetText( "Press E to enter build mode" );
						}						
					break;

					case SDLK_ESCAPE:
						if( mainPlayer.hasDisabledMouseMovement() )
						{
							mouse->visible = true;
							quitButton->visible = true;
							shadeScreen->visible = true;
							mainController.PauseObjects();
							worldEnvironment.Pause();
						}
						else
						{
							mouse->visible = false;
							quitButton->visible = false;
							shadeScreen->visible = false;
							mainController.UnpauseObjects();
							worldEnvironment.Unpause();
						}
					break;
				}
			}

			if( event.type == SDL_MOUSEBUTTONDOWN )
			{

				if( event.button.button == SDL_BUTTON_LEFT && mainPlayer.isFlying() )
					mouseButton[0] = true;

				if( event.button.button == SDL_BUTTON_RIGHT && mainPlayer.isFlying() )
					mouseButton[1] = true;

				if( ( event.button.button == SDL_BUTTON_WHEELUP || event.button.button == SDL_BUTTON_WHEELDOWN ) && mainPlayer.isFlying() )
				{
					BrushSize = mainPlayer.GetPosition().y + 1;
					if( BrushSize < 5.0 )
						BrushSize = 5.0;

					std::stringstream ss;
					ss << "Height Position: " << mainPlayer.GetPosition().y << ", Brush Size: " << BrushSize;
					barTextBox->SetText( ss.str() );
					newBar->SetValue( mainPlayer.GetPosition().y );
				}
			}

			if( event.type == SDL_MOUSEBUTTONUP )
			{
				if( event.button.button == SDL_BUTTON_LEFT )
					mouseButton[0] = false;

				if( event.button.button == SDL_BUTTON_RIGHT )
					mouseButton[1] = false;
			}

		}
		
		int meshTickes = dynamicMeshTimer.get_ticks();

		// Left Mouse Button Event
		if( mouseButton[0] && !mouse->visible )
		{		
			std::vector<StaticObject*> staticObjects = worldTerrain.GetClosestChunks();
			for( auto iter = staticObjects.begin(); iter != staticObjects.end(); iter++ )
			{
				switch( newToolBelt->GetSelectedTool() )
				{
					case 0: // Press
						DynamicMesh::addPress( mainPlayer.GetPosition(), 30.0, BrushSize, 10.0, 1 * Math::timeFactor( meshTickes ), (*iter), MeshFunctions::ChangeFunction1 );
					break;

					case 1: // Sin 5.0 amp
						DynamicMesh::addSin( mainPlayer.GetPosition(), BrushSize, 5.0, 1, 1, (*iter), MeshFunctions::ChangeFunction1 );
					break;

					case 2: // Flatten, rate
						DynamicMesh::Flatten( mainPlayer.GetPosition(), BrushSize, -0.5, 1, (*iter), MeshFunctions::ChangeFunction1 );
					break;

					case 3: // Press high pressure
						DynamicMesh::addPress( mainPlayer.GetPosition(), 80.0, BrushSize, 10.0, 1 * Math::timeFactor( meshTickes ), (*iter), MeshFunctions::ChangeFunction1 );
					break;
				}
			}		
		}

		// Right Mouse Button Event
		if( mouseButton[1] && !mouse->visible )
		{
			std::vector<StaticObject*> staticObjects = worldTerrain.GetClosestChunks();
			for( auto iter = staticObjects.begin(); iter != staticObjects.end(); iter++ )
			{
				switch( newToolBelt->GetSelectedTool() )
				{
					case 0: // Press reverse
						DynamicMesh::addPress( mainPlayer.GetPosition(), 30.0, BrushSize, 10.0, -1 * Math::timeFactor( meshTickes ), (*iter), MeshFunctions::ChangeFunction1 );
					break;

					case 1: // Sin 5/2 amp
						DynamicMesh::addSin( mainPlayer.GetPosition(), BrushSize, 2.5, 1, 1, (*iter), MeshFunctions::ChangeFunction1 );
					break;

					case 2: // Flatten, abs
						DynamicMesh::Flatten( mainPlayer.GetPosition(), BrushSize, -0.5, 100.0, (*iter), MeshFunctions::ChangeFunction1 );
					break;

					case 3: // Press reverse crazy presure
						DynamicMesh::addPress( mainPlayer.GetPosition(), 400.0, BrushSize, 10.0, 1 * Math::timeFactor( meshTickes ), (*iter), MeshFunctions::ChangeFunction1 );
					break;
				}
			}
		}
		dynamicMeshTimer.start();
		
		Camera::resetCamera();
		DisplayScreen::SwapFrameBuffers();

	} // End of main loop

	if( !WORLD_READ_ONLY )
	{
		SaveWorld( worldTerrain, "world1" );     
	}

	CleanUp();      

	return 0;    
}
