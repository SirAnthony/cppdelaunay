/*
 * HalfedgePriorityQueue.h
 *
 *  Created on: 07.03.2013
 *
 */

#ifndef DELAUNAY_HALFEDGEPRIORITYQUEUE_H_
#define DELAUNAY_HALFEDGEPRIORITYQUEUE_H_

#include "delaunay/Halfedge.h"
#include <vector>
#include "defines.h"

namespace Delaunay
{

	class HalfedgePriorityQueue
	{
	public:
		HalfedgePriorityQueue( Number, Number, int );
		~HalfedgePriorityQueue( );

		void insert( Halfedge* halfEdge );
		void remove( Halfedge* halfEdge );
		bool empty();

		/**
		 * @return coordinates of the Halfedge's vertex in V*, the transformed Voronoi diagram
		 *
		 */
		Point* min();

		/**
				 * remove and return the min Halfedge
				 * @return
				 *
				 */
		Halfedge* extractMin();


	private:
		void initialize();
		int bucket( Halfedge* halfEdge );
		bool isEmpty( int );

		/**
		 * move _minBucket until it contains an actual Halfedge (not just the dummy at the top);
		 *
		 */
		void adjustMinBucket();

		std::vector<Halfedge*> _hash;
		int _count;
		int _minBucket;
		int _hashsize;

		Number _ymin;
		Number _deltay;

	};
} /* namespace Delaunay */
#endif /* DELAUNAY_HALFEDGEPRIORITYQUEUE_H_ */
