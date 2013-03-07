/*
 * EdgeReorderer.h
 *
 *  Created on: 05.03.2013
 *
 */

#ifndef DELAUNAY_EDGEREORDERER_H_
#define DELAUNAY_EDGEREORDERER_H_

#include "delaunay/LR.h"
#include "delaunay/Edge.h"
#include <vector>

namespace Delaunay {

	class EdgeReorderer
	{
	public:
		enum Criterion {
			cVertex, cSite
		};

		EdgeReorderer( const std::vector< Edge* >&, Criterion );
		~EdgeReorderer( );

		inline std::vector< Edge* >&edges( )
		{
			return _edges;
		}

		inline std::vector< LR::Side > edgeOrientations( )
		{
			return _edgeOrientations;
		}

	private:

		std::vector< Edge* > _edges;
		std::vector< LR::Side > _edgeOrientations;

		void reorderEdges( const std::vector< Edge* >&, Criterion );

	};
}

#endif /* DELAUNAY_EDGEREORDERER_H_ */
