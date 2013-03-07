/*
 * LineSegment.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "LineSegment.h"

namespace Delaunay
{

	LineSegment::LineSegment( const Point* pt0, const Point* pt1 ) : p0(pt0), p1(pt1) {
		// TODO Auto-generated constructor stub

	}

	LineSegment::~LineSegment( ) {
		// TODO Auto-generated destructor stub
	}

	Number compareLengths_MAX( const LineSegment* segment0, const LineSegment* segment1 )
	{
		Number length0 = Point::distance( segment0->p0, segment0->p1 );
		Number length1 = Point::distance( segment1->p0, segment1->p1 );
		if( length0 < length1 )
			return 1;
		if( length0 > length1 )
			return -1;
		return 0;
	}

	Number compareLengths( const LineSegment* s0, const LineSegment* s1 )
	{
		return -compareLengths_MAX( s0, s1 );
	}

} /* namespace Delaunay */
