/*
 * Triangle.h
 *
 *  Created on: 06.03.2013
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "delaunay/Site.h"

namespace Delaunay {

	class Triangle
	{
	public:
		Triangle( Site* a, Site* b , Site* c );
		Site** sites(  );

	private:
		Site* _sites[3];
	};

} /* namespace Delaunay */
#endif /* TRIANGLE_H_ */
