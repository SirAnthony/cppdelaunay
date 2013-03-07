/*
 * Rectangle.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "Rectangle.h"

namespace Delaunay
{

	Rectangle::Rectangle( Number xmin, Number ymin, Number w, Number h )
	{
		X = xmin;
		Y = ymin;
		Width = w;
		Height = h;
	}

	Rectangle::Rectangle( const Rectangle& r )
	{
		X = r.x();
		Y = r.y();
		Width = r.width();
		Height = r.height();
	}

	Rectangle::~Rectangle( )
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace Delaunay */
