/*
 * EdgeReorderer.cpp
 *
 *  Created on: 05.03.2013
 *
 */

#include "delaunay/EdgeReorderer.h"
#include "delaunay/Vertex.h"
#include "delaunay/Site.h"
#include <assert.h>
#include <deque>

namespace Delaunay {

	EdgeReorderer::EdgeReorderer( const std::vector<Edge*>& origEdges,
			Criterion criterion )
	{
		assert( criterion == cVertex || criterion == cSite );
		if( origEdges.size() > 0 )
			reorderEdges( origEdges, criterion );
	}

	EdgeReorderer::~EdgeReorderer( )
	{
		_edges.clear();
		_edgeOrientations.clear();
	}

	void EdgeReorderer::reorderEdges( const std::vector<Edge*>& origEdges,
			Criterion criterion )
	{
		int n = origEdges.size();
		Edge* edge;

		// we're going to reorder the edges in order of traversal
		std::vector<bool> done( n );
		std::fill( done.begin(), done.end(), false );

		std::deque<Edge*> newEdges;

		edge = origEdges[0];
		newEdges.push_back( edge );
		_edgeOrientations.push_back( LR::LEFT );

#define GET_POINTS( l, r )	\
	ICoord* l = (criterion == cVertex) ? 					\
			reinterpret_cast<ICoord*>(edge->leftVertex()) :	\
			reinterpret_cast<ICoord*>(edge->leftSite()); 	\
	ICoord* r = (criterion == cVertex) ? 					\
			reinterpret_cast<ICoord*>(edge->rightVertex()) :\
			reinterpret_cast<ICoord*>(edge->rightSite()); 	\
	if( l == Vertex::VERTEX_AT_INFINITY || r == Vertex::VERTEX_AT_INFINITY )		\
		return;


		GET_POINTS( firstPoint, lastPoint )

		if( firstPoint == Vertex::VERTEX_AT_INFINITY
				|| lastPoint == Vertex::VERTEX_AT_INFINITY )
			return;

		done[0] = true;

		int nDone = 1;
		while( nDone < n ){
			for( int i = 1; i < n; ++i ){
				if( done[i] )
					continue;

				edge = origEdges[i];
				GET_POINTS( leftPoint, rightPoint )

				if( leftPoint == lastPoint ){
					lastPoint = rightPoint;
					_edgeOrientations.push_back( LR::LEFT );
					newEdges.push_back( edge );
					done[i] = true;
				}else if( rightPoint == firstPoint ){
					firstPoint = leftPoint;
					_edgeOrientations.insert( _edgeOrientations.begin(), LR::LEFT );
					newEdges.push_front( edge );
					done[i] = true;
				}else if( leftPoint == firstPoint ){
					firstPoint = rightPoint;
					_edgeOrientations.insert( _edgeOrientations.begin(), LR::RIGHT );
					newEdges.push_front( edge );
					done[i] = true;
				}else if( rightPoint == lastPoint ){
					lastPoint = leftPoint;
					_edgeOrientations.push_back( LR::RIGHT );
					newEdges.push_back( edge );
					done[i] = true;
				}
				if( done[i] ){
					++nDone;
				}
			}
		}

		_edges.resize( newEdges.size() );
		std::copy( newEdges.begin(), newEdges.end(),
				std::inserter( _edges, _edges.begin() ) );
	}
}
