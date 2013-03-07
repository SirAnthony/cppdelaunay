/*
 * LineSegment.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef DELAUNAY_LINESEGMENT_H_
#define DELAUNAY_LINESEGMENT_H_

#include "geom/Point.h"

namespace Delaunay
{

	class LineSegment
	{
	public:
		LineSegment( const Point*, const Point* );
		~LineSegment( );

		static Number compareLengths_MAX( const LineSegment*, const LineSegment* );
		static Number compareLengths( const LineSegment*, const LineSegment* );

		const Point* p0;
		const Point* p1;
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_LINESEGMENT_H_ */
