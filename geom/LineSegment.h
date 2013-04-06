/*
 * LineSegment.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef DELAUNAY_LINESEGMENT_H_
#define DELAUNAY_LINESEGMENT_H_

#include "geom/Point.h"
#include <list>

namespace Delaunay
{

	class LineSegment
	{
	public:
		static LineSegment* create( const Point*, const Point* );
		static void clean();
		void dispose();

		static Number compareLengths_MAX( const LineSegment*, const LineSegment* );
		static Number compareLengths( const LineSegment*, const LineSegment* );

		const Point* p0;
		const Point* p1;
	private:
		LineSegment( const Point*, const Point* );
		~LineSegment( );
		void init( const Point*, const Point* );

		static std::list< LineSegment* > _pool;
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_LINESEGMENT_H_ */
