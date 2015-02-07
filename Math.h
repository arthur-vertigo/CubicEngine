/***

Math: Common math functions

***/

#ifndef MATH_H
#define MATH_H

#include "SDL.h"
#include "Point.h"
#include "Dimensions.h"
#include <ctime>
#include <cmath>

const double DEG_TO_RAD = 3.14159 / 180;
const double RAD_TO_DEG = 180 / 3.14159;

class Math
{
public:

	// Convert deg to rad
	static double DegToRad( double deg )
	{
		return deg * DEG_TO_RAD;
	}

	// Convert rad to deg
	static double RadToDeg( double rad )
	{
		return rad * RAD_TO_DEG;
	}

	// Returns a random double in the range [min, max] with 2 decimal places
	static double randomDouble( double min, double max )
	{
		if( max == 0.0 )
			return 0.0;

		min *= 100;
		max *= 100;
		return ( min + rand() % static_cast<int>(max - min + 1) ) / 100.0;

	}

	// Returns a random float in the range [min, max] with 1 decimal places
	static float RandomFloat( float min, float max )
	{
		if( max == 0.f )
			return 0.f;

		min *= 10;
		max *= 10;
		return ( min + rand() % static_cast<int>(max - min + 1) ) / 10.f;

	}

	// Returns a random int [min, max]
	static int RandomInt( int min, int max )
	{
		if( max == 0 )
			return 0;

		return min + rand() % (max - min + 1);
	}

	// Returns the amount to add for x per second
	static double timeFactor( int ticks )
	{
		return ticks / 1000.0;
	} 

	// Returns the area of a triangle with SSS
	static double areaOfTriangleSSS( double a, double b, double c )
	{
		double cosC = cos( (a*a + b*b + c*c) / (2 * a * b) );
		double angleC = acos( cosC );
		return (.5 * a * b * sin( angleC ));
	}

	// Returns the height of point x,d in a triangle p1 p2 p3
	static double calcY(Point3D p1, Point3D p2, Point3D p3, double x, double z) 
	{
		double det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
     
		double l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
		double l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
		double l3 = 1.0f - l1 - l2;
     
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	// Returns the normal for a triangle v1 v2 v3
	static Point3D calcNormal( Point3D v1, Point3D v2, Point3D v3 )
	{

		Point3D v4 = subVectors( v1, v2 );
		Point3D v5 = subVectors( v2, v3 );

		Point3D v6 = crossProduct( v4, v5 );

		normalizeVector( v6 );

		if( v6.y < 0 )
		{
			v6.y = v6.y;
		}

		return v6;
	}

	// Returns the 3d crossProduct vector of v1 and v2
	static Point3D crossProduct(Point3D v1, Point3D v2) 
	{
		Point3D normalVector;

		// Cross product 
		normalVector.x = v1.y * v2.z - v1.z * v2.y;
		normalVector.y = v2.x * v1.z - v1.x * v2.z;
		normalVector.z = v1.x * v2.y - v1.y * v2.x;

		return normalVector;
	}

	// Will return a vector that adds v1 and v2
	static Point3D addVectors( Point3D v1, Point3D v2 )
	{
		Point3D addVector( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
		return addVector;
	}

	// Will return a vector that subtracts v1 and v2
	static Point3D subVectors( Point3D v1, Point3D v2 )
	{
		Point3D subVector( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
		return subVector;
	}

	// Returns the dot product of vector v1 and v2. Only 
	// calculates the x and y
	static double dotProduct2D( Point3D v1, Point3D v2 )
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	// Returns the dot product of vector v1 and v2. 
	static double dotProduct3D( Point3D v1, Point3D v2 )
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	// Will normalize a provided vector
	static void normalizeVector( Point3D& v1 )
	{
		double mag = sqrt( ( v1.x * v1.x ) + ( v1.y * v1.y ) + ( v1.z * v1.z ) );
		v1.x /= mag;
		v1.y /= mag;
		v1.z /= mag;
	}

	// Returns the center of a triangle with 3x Point3D
	static Point3D getTriangleCenter( Point3D p1, Point3D p2, Point3D p3 )
	{
		return Point3D( p1.x + p2.x + p3.x, p1.y + p2.y + p3.y, p1.z + p2.z + p3.z );
	}

	// Returns the size of a triangle gives its 3 points *** REFFFFACCCTTOOORRR!!!!***
	static Size3D GetTriangleSize( Point3D p1, Point3D p2, Point3D p3 )
	{
		double lowX = p1.x;
		double highX = p1.x;

		double lowY = p1.y;
		double highY = p1.y;

		double lowZ = p1.z;
		double highZ = p1.z;

		if( p2.x < lowX ){ lowX = p2.x; }
		if( p3.x < lowX ){ lowX = p3.x; }

		if( p2.x > highX ){ highX = p2.x; }
		if( p3.x > highX ){ highX = p3.x; }

		if( p2.y < lowY ){ lowY = p2.y; }
		if( p3.y < lowY ){ lowY = p3.y; }

		if( p2.y > highY ){ highY = p2.y; } // ABSOLUTE SHIT
		if( p3.y > highY ){ highY = p3.y; }

		if( p2.z < lowZ ) { lowZ = p2.z; }
		if( p3.z < lowZ ) { lowZ = p3.z; }

		if( p2.z > highZ ) { highZ = p2.z; }
		if( p3.z > highZ ) { highZ = p3.z; }

		return Size3D( abs(highX - lowX), abs(highY - lowY), abs(highZ - lowZ) );
	}

	// Returns the hyp of a right triangle
	static double pythag2D( Point3D p1 )
	{
		return sqrt( (p1.x * p1.x) + ( p1.y * p1.y) );
	}

	// Calculates the magnitude of a vector
	static double pythag3D( Point3D p1 )
	{
		return sqrt( (p1.x * p1.x) + ( p1.y * p1.y) + ( p1.z * p1.z ) );
	}

	// Calculates area of a PPP triangle
	static float areaOfTrianglePPP( Point3D p1, Point3D p2, Point3D p3 )
	{
		float det = 0.0;
		det = static_cast<float>( abs(((p1.x - p3.x) * (p2.z - p3.z)) - ((p2.x - p3.x) * (p1.z - p3.z))) );
		return (det / 2.f);
	}

	// Returns the orientation of a vector to two lines
	static double sign(Point3D p1, Point3D p2, Point3D p3)
	{
	  return (p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z);
	}

	// Returns true if p is in the triangle ABC with a sign test
	static bool pointInTriangleSign( Point3D p, Point3D A, Point3D B, Point3D C )
	{
	  bool b1, b2, b3;

	  b1 = sign(p, A, B) < 0.0;
	  b2 = sign(p, B, C) < 0.0;
	  b3 = sign(p, C, A) < 0.0;

	  return ((b1 == b2) && (b2 == b3));
	}
 
	// Returns true if p is in triangle ABC with an area test
	static bool pointInTriangleArea(Point3D P,Point3D A,Point3D B,Point3D C)
    {
		double ABC = areaOfTrianglePPP( A, B, C );

		double PBC = areaOfTrianglePPP( P, B, C );
		double APC = areaOfTrianglePPP( A, P, C );
		double ABP = areaOfTrianglePPP( A, B, P );

		if( PBC + APC + ABP - ABC < .01 )
			return true;
		return false;
    }	
};

#endif // !MATH_H
