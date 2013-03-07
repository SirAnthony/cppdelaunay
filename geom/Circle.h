/*
 * Circle.h
 *
 *  Created on: 06.03.2013
 */

#ifndef DELAUNAY_CIRCLE_H_
#define DELAUNAY_CIRCLE_H_

#include "geom/Point.h"
#include "defines.h"


namespace Delaunay {

	class Circle
	{
	public:
		Circle( Number, Number, Number );
		~Circle( );

		Point* center;
		Number radius;
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_CIRCLE_H_ */
