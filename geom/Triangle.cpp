/*
 * Triangle.cpp
 *
 *  Created on: 06.03.2013
 */

#include "Triangle.h"

namespace Delaunay {

	Triangle::Triangle( Site* a, Site* b , Site* c  )
	{
		_sites[0] = a;
		_sites[1] = b;
		_sites[2] = c;
	}

	Site** Triangle::sites(  )
	{
		return _sites;
	}

} /* namespace Delaunay */
