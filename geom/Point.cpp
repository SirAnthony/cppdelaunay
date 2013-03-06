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
		return sqrt(
				(v1->x - v2->x) * (v1->x - v2->x)
						+ (v1->y - v2->y) * (v1->y - v2->y) );
	}

} /* namespace Delaunay */
