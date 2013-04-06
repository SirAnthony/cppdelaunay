/*
 * EdgeList.h
 *
 *  Created on: 07.03.2013
 *
 */

#ifndef DELAUNAY_EDGELIST_H_
#define DELAUNAY_EDGELIST_H_

#include "delaunay/Halfedge.h"
#include "defines.h"
#include <vector>

namespace Delaunay
{

	class EdgeList
	{
	public:
		EdgeList( Number, Number, int );
		~EdgeList( );
		Halfedge* leftEnd( );
		Halfedge* rightEnd( );

		void dispose( );

		/**
		 * Insert newHalfedge to the right of lb
		 * @param lb
		 * @param newHalfedge
		 *
		 */
		void insert( Halfedge*, Halfedge* );

		/**
		 * This function only removes the Halfedge from the left-right list.
		 * We cannot dispose it yet because we are still using it.
		 * @param halfEdge
		 *
		 */
		void remove( Halfedge* );

		/**
		 * Find the rightmost Halfedge that is still left of p
		 * @param p
		 * @return
		 *
		 */
		Halfedge* edgeListLeftNeighbor( const Point* );

		/* Get entry from hash table, pruning any deleted nodes */
		Halfedge* getHash( int );

	private:
		Number _deltax;
		Number _xmin;

		int _hashsize;
		std::vector< Halfedge* > _hash;
		Halfedge* _leftEnd;
		Halfedge* _rightEnd;
	};

} /* namespace Delaunay */
#endif /* DELAUNAY_EDGELIST_H_ */
