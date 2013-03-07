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
		const Point* coord( ) const
		{
			return NULL;
		}
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_ICOORD_H_ */
