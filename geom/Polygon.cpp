/*
 * Polygon.cpp
 *
 *  Created on: 06.03.2013
 */

#include "Polygon.h"
#include <math.h>

namespace Delaunay {

	Polygon::Polygon( std::vector<Point*>& vertices )
	{
		_vertices = vertices;
	}

	Polygon::~Polygon( )
	{
	}

	Number Polygon::area( )
	{
		return abs( signedDoubleArea() * 0.5 );
	}

	Winding::Direction Polygon::winding( )
	{
		Number signedDoubleArea = signedDoubleArea();
		if( signedDoubleArea < 0 )
			return Winding::CLOCKWISE;
		if( signedDoubleArea > 0 )
			return Winding::COUNTERCLOCKWISE;
		return Winding::NONE;
	}

	Number Polygon::signedDoubleArea( )
	{
		unsigned index, nextIndex;
		unsigned n = _vertices.size();
		Point* point;
		Point* next;
		Number signedDoubleArea = 0;
		for( index = 0; index < n; ++index ){
			nextIndex = ( index + 1 ) % n;
			point = _vertices[index];
			next = _vertices[nextIndex];
			signedDoubleArea += point->x * next->y - next->x * point->y;
		}
		return signedDoubleArea;
	}

}

/* namespace Delaunay */
