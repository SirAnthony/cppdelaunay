/*
 * Polygon.h
 *
 *  Created on: 06.03.2013
 */

#ifndef DELAUNAY_POLYGON_H_
#define DELAUNAY_POLYGON_H_

#include "geom/Point.h"
#include "defines.h"
#include <vector>

namespace Delaunay
{

	namespace Winding
	{
		enum Direction
		{
			NONE = 0, CLOCKWISE, COUNTERCLOCKWISE,
		};
	}

	class Polygon
	{
	public:
		Polygon( std::vector< Point* >& vertices );
		~Polygon( );
		Number area( );
		Winding::Direction winding( );
		Number signedDoubleArea( );

	private:
		std::vector< Point* > _vertices;
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_POLYGON_H_ */
