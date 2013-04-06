/*
 * ICoord.h
 *
 *  Created on: 06.03.2013
 */

#ifndef DELAUNAY_ICOORD_H_
#define DELAUNAY_ICOORD_H_

#include "geom/Point.h"

namespace Delaunay
{

	class ICoord
	{
	public:
		ICoord() :  _coord(NULL) {};

		inline const Point* coord( ) const
		{
			return _coord;
		}

		inline Number x( ) const
		{
			return _coord->x;
		}
		inline Number y( ) const
		{
			return _coord->y;
		}

		inline Number dist( ICoord* p )
		{
			return Point::distance( p->coord( ), _coord );
		}

	protected:
		Point* _coord;
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_ICOORD_H_ */
