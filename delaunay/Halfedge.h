/*
 * Halfedge.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef HALFEDGE_H_
#define HALFEDGE_H_

#include "geom/Point.h"
#include "delaunay/Edge.h"
#include "delaunay/LR.h"
#include "delaunay/Vertex.h"


namespace Delaunay
{

	class Halfedge
	{
	public:
		Halfedge* create( Edge* edge, LR::Side lr );
		Halfedge* createDummy();
		void dispose();
		void reallyDispose();

		bool isLeftOf( Point* p );

		Halfedge* edgeListLeftNeighbor;
		Halfedge* edgeListRightNeighbor;
		Halfedge* nextInPriorityQueue;

		Edge* edge;
		LR::Side leftRight;
		Vertex* vertex;

		// the vertex's y-coordinate in the transformed Voronoi space V*
		Number ystar;

	private:
		Halfedge( Edge* edge, LR::Side lr );
		~Halfedge( );

		static std::vector< Halfedge* > _pool;

		Halfedge* init( Edge* edge, LR::Side lr );
	};

} /* namespace Delaunay */
#endif /* HALFEDGE_H_ */
