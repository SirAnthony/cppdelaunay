/*
 * Point.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef DELAUNAY_POINT_H_
#define DELAUNAY_POINT_H_

#include "defines.h"

namespace Delaunay
{

	class Point
	{
	public:
		Point( Number, Number );
		~Point( );

		Number x, y;

		/*
		 * Returns the distance between pt1 and pt2.
		 */
		static Number distance( const Point*, const Point* );
		static Point* interpolate( const Point*, const Point*, Number delta );


	};

} /* namespace Delaunay */
#endif /* DELAUNAY_POINT_H_ */
