/*
 * Edge.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef DELAUNAY_EDGE_H_
#define DELAUNAY_EDGE_H_

#include "geom/LineSegment.h"
#include "delaunay/Vertex.h"
#include "delaunay/LR.h"
#include "delaunay/Site.h"
#include "defines.h"
#include <vector>
#include <list>


namespace Delaunay
{

	class Edge
	{

		/**
		 * The line segment connecting the two Sites is part of the Delaunay triangulation;
		 * the line segment connecting the two Vertices is part of the Voronoi diagram
		 *
		 */

	public:
		static void clean();


		LineSegment* delaunayLine( );
		LineSegment* voronoiEdge( );

		/**
		 * This is the only way to create a new Edge
		 * @param site0
		 * @param site1
		 * @return
		 *
		 */
		static Edge* createBisectingEdge( Site*, Site* );
		void dispose( );

		inline Vertex* leftVertex( )
		{
			return _leftVertex;
		}

		inline Vertex* rightVertex( )
		{
			return _rightVertex;
		}

		inline Vertex* vertex( LR::Side leftRight )
		{
			return ( leftRight == LR::LEFT ) ? _leftVertex : _rightVertex;
		}

		void setVertex( LR::Side leftRight, Vertex* v );

		inline bool isPartOfConvexHull( )
		{
			return ( _leftVertex == NULL || _rightVertex == NULL );
		}

		Number sitesDistance( );

		static Number compareSitesDistances_MAX( Edge* edge0, Edge* edge1 );
		static Number compareSitesDistances( Edge* edge0, Edge* edge1 );

		inline Point** clippedEnds( )
		{
			return _clippedVertices;
		}

		// unless the entire Edge is outside the bounds.
		// In that case visible will be false:
		inline bool visible( )
		{
			return ( _clippedVertices[LR::LEFT] != NULL
					&& _clippedVertices[LR::RIGHT] != NULL );
		}

		inline void leftSite( Site* s )
		{
			_sites[LR::LEFT] = s;
		}
		inline Site* leftSite( )
		{
			return _sites[LR::LEFT];
		}

		inline void rightSite( Site* s )
		{
			_sites[LR::RIGHT] = s;
		}
		inline Site* rightSite( )
		{
			return _sites[LR::RIGHT];
		}

		Site* site( LR::Side leftRight )
		{
			return _sites[leftRight];
		}

		/**
		 * Set _clippedVertices to contain the two ends of the portion of the Voronoi edge that is visible
		 * within the bounds.  If no part of the Edge falls within the bounds, leave _clippedVertices null.
		 * @param bounds
		 *
		 */
		void clipVertices( const Rectangle& bounds );

		void out();
		void outEnd();

		// the equation of the edge: ax + by = c
		Number a, b, c;

		static Edge* DELETED;

	private:
		Edge( );
		~Edge( );

		static std::list< Edge* > _pool;

		static Edge* create( );
		void init( );

		static int _nedges;

		// the two Voronoi vertices that the edge connects
		//		(if one of them is null, the edge extends to infinity)
		Vertex* _leftVertex;
		Vertex* _rightVertex;

		// Once clipVertices() is called, this Dictionary will hold two Points
		// representing the clipped coordinates of the left and right ends...
		Point* _clippedVertices[2];

		// the two input Sites for which this Edge is a bisector:
		Site* _sites[2];
		int _edgeIndex;

	};

} /* namespace Delaunay */
#endif /* DELAUNAY_EDGE_H_ */
