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
		Number static distance( const Point*, const Point* );


	};

} /* namespace Delaunay */
#endif /* DELAUNAY_POINT_H_ */
