/*
 * ICoord.h
 *
 *  Created on: 06.03.2013
 */

#ifndef ICOORD_H_
#define ICOORD_H_

#include "geom/Point.h"

namespace Delaunay {

	class ICoord
	{
		inline const Point* coord() const;
	};

} /* namespace Delaunay */
#endif /* ICOORD_H_ */
