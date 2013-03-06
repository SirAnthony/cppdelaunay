/*
 * Circle.h
 *
 *  Created on: 06.03.2013
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

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
#endif /* CIRCLE_H_ */
