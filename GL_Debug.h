/***

GL_DEBUG: Allows debugging functionality
           -Mainly used to draw verts and normals
			
***/

#ifndef GL_DEBUG_H
#define GL_DEBUG_H

#include "SDL.h"
#include "SDL_opengl.h"
#include "Point.h"
#include "Vector.h"
#include "Math.h"

class GLDebug
{
public:
	enum OPTIONAL_DRAW_MODES
	{
		DEFAULT,
		SQUARE
	};

	// Will draw vector at point p
	static void drawPoint3D( Point3D p, Size3D scale, Vector3D v, double length = 1 )
	{
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );
		glDisable( GL_TEXTURE_2D );
			glBegin( GL_LINES );

				double r = 0.0, g = 0.0, b = 0.0;

				if( v.x < 0 )
					r = 0.5;
				if( v.y < 0 )
					g = 0.5;
				if( v.z < 0 )
					b = 0.5;

				//glColor4d( r, g, b, 1.0 );

				glVertex3d( ( p.x ) * scale.w, ( p.y ) * scale.h, ( p.z ) * scale.d );
				glVertex3d( ( p.x + ( v.x * length ) ) * scale.w, ( p.y + ( v.y * length ) ) * scale.h, ( p.z + ( v.z * length ) ) * scale.d );
			glEnd();
		glEnable( GL_LIGHTING );
		glEnable( GL_FOG );
	}

	// Will draw point p
	static void drawPoint3D( Point3D p, Size3D scale, double length, int optional_draw_mode = 0 )
	{
		glDisable( GL_LIGHTING );
		glDisable( GL_FOG );
		glDisable( GL_TEXTURE_2D );
			glBegin( GL_LINES );

				double r = 0.5, g = 0.5, b = 0.5;

				if( p.x < 0 )
					r = 1.0;
				if( p.y < 0 )
					g = 1.0;
				if( p.z < 0 )
					b = 1.0;

				//glColor4d( r, g, b, 1.0 );
			
				switch( optional_draw_mode )
				{
					case SQUARE:

						glVertex3d( p.x-length, p.y+length, p.z ); glVertex3d( p.x+length, p.y+length, p.z );
						glVertex3d( p.x-length, p.y-length, p.z ); glVertex3d( p.x+length, p.y-length, p.z );
						glVertex3d( p.x-length, p.y+length, p.z ); glVertex3d( p.x-length, p.y-length, p.z );
						glVertex3d( p.x+length, p.y+length, p.z ); glVertex3d( p.x+length, p.y-length, p.z );

					break;
					case DEFAULT:
					default:
						glVertex3d( p.x-length, p.y, p.z ); glVertex3d( p.x+length, p.y, p.z );
						glVertex3d( p.x, p.y-length, p.z ); glVertex3d( p.x, p.y+length, p.z ); // Need to add scale change
						glVertex3d( p.x, p.y, p.z-length ); glVertex3d( p.x, p.y, p.z+length );
					break;
				}
			glEnd();
		glEnable( GL_LIGHTING );
		glEnable( GL_FOG );
	}
};
#endif // !GL_DEBUG_H
