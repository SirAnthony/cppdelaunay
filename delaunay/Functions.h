/*
 * d.h
 *
 *  Created on: 06.03.2013
 */

#ifndef DELAUNAY_FUNCTIONS_H_
#define DELAUNAY_FUNCTIONS_H_

#include "geom/Point.h"
#include "geom/LineSegment.h"
#include "delaunay/Edge.h"
#include <vector>

namespace Delaunay {

	std::vector< LineSegment* > delaunayLinesForEdges(std::vector< Edge* >&);
	std::vector< LineSegment* > visibleLineSegments(std::vector< Edge* >&);
	std::vector< Edge* > selectEdgesForSitePoint( const Point*, std::vector< Edge* >& );
	std::vector< Edge* > selectNonIntersectingEdges( /*keepOutMask:BitmapData,*/ std::vector< Edge* >& );

} /* namespace Delaunay */
#endif /* DELAUNAY_FUNCTIONS_H_ */
