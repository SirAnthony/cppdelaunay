/*
 * Vertex.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef DELAUNAY_VERTEX_H_
#define DELAUNAY_VERTEX_H_

#include "delaunay/ICoord.h"
#include <vector>

namespace Delaunay
{

	class Halfedge;

	class Vertex : public ICoord
	{
	public:
		static Vertex* create( Number x, Number y);
		void dispose();

		inline const Point* coord() const
		{
			return _coord;
		}
		inline int vertexIndex( )
		{
			return _vertexIndex;
		}

		void setIndex();

		inline Number x( )
		{
			return _coord->x;
		}
		inline Number y( )
		{
			return _coord->y;
		}

		/**
		 * This is the only way to make a Vertex
		 *
		 * @param halfedge0
		 * @param halfedge1
		 * @return
		 *
		 */
		static Vertex* intersect( Halfedge* halfedge0, Halfedge* halfedge1 );

		static Vertex* VERTEX_AT_INFINITY;

	private:
		Vertex( Number, Number );
		~Vertex( );
		Vertex* init( Number x, Number y );

		static std::vector< Vertex* > _pool;
		static int _nvertices;
		Point* _coord;
		int _vertexIndex;


	};

} /* namespace Delaunay */
#endif /* DELAUNAY_VERTEX_H_ */
