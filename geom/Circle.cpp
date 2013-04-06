/*
 * Circle.cpp
 *
 *  Created on: 06.03.2013
 */

#include "geom/Circle.h"

namespace Delaunay {

	Circle::Circle( Number centerX, Number centerY, Number r )
	{
		center = Point::create(centerX, centerY);
		radius = r;

	}

	Circle::~Circle( )
	{
		center->dispose();
	}

} /* namespace Delaunay */
