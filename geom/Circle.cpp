/*
 * Circle.cpp
 *
 *  Created on: 06.03.2013
 */

#include "geom/Circle.h"

namespace Delaunay {

	Circle::Circle( Number centerX, Number centerY, Number r )
	{
		center = new Point(centerX, centerY);
		radius = r;

	}

	Circle::~Circle( )
	{
		delete center;
	}

} /* namespace Delaunay */
