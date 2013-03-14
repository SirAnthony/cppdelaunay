/*
 * Point.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "Point.h"
#include <math.h>

namespace Delaunay
{

	Point::Point( Number X, Number Y ) : x( X ), y( Y ) {

	}

	Point::~Point( ) {
		// TODO Auto-generated destructor stub
	}

	Number Point::distance( const Point* v1, const Point* v2 ) {
		float dx = v1->x - v2->x;
		float dy = v1->y - v2->y;
		return sqrt( dx * dx + dy * dy );
	}

	Point* interpolate( const Point* first, const Point* second, Number delta )
	{
		return new Point( first->x + delta * ( second->x - first->x ),
				first->y + delta * ( second->y - first->y ) );
	}

} /* namespace Delaunay */
