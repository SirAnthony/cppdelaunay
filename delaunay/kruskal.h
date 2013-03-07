/*
 * kruskal.h
 *
 *  Created on: 07.03.2013
 *
 */

#ifndef DELAUNAY_KRUSKAL_H_
#define DELAUNAY_KRUSKAL_H_

#include "geom/LineSegment.h"
#include <vector>

namespace Delaunay
{

	enum KruskalType
	{
		ktMinimum = 0, ktMaximum
	};

	/**
	 *  Kruskal's spanning tree algorithm with union-find
	 * Skiena: The Algorithm Design Manual, p. 196ff
	 * Note: the sites are implied: they consist of the end points of the line segments
	 */
	std::vector< LineSegment* > kruskal( std::vector< LineSegment* >& lineSegments,
			enum KruskalType type = ktMinimum );

	class Node
	{
	public:
		Node( );

		static std::vector< Node* > pool;
		Node* parent;
		int treeSize;
	};

	Node* find( Node* node );

} /* namespace Delaunay */
#endif /* DELAUNAY_KRUSKAL_H_ */
