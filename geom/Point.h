/*
 * Point.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef DELAUNAY_POINT_H_
#define DELAUNAY_POINT_H_

#include "defines.h"
#include <list>

namespace Delaunay
{

	class Point
	{
	public:
		static Point* create( Number, Number );
		static void clean();
		void dispose();

		Number x, y;


		/*
		 * Returns the distance between pt1 and pt2.
		 */
		static Number distance( const Point*, const Point* );
		static Point* interpolate( const Point*, const Point*, Number delta );
	private:
		Point( Number, Number );
		~Point( );
		void init( Number, Number );

		static std::list< Point* > _pool;
		static int _count;
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_POINT_H_ */
