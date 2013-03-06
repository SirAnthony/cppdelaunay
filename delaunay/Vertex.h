/*
 * Vertex.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>

namespace Delaunay
{

	class Halfedge;

	class Vertex
	{
	public:
		static Vertex* create( Number x, Number y);
		void dispose();

		inline Point* coord()
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
		Vertex* intersect( Halfedge* halfedge0, Halfedge* halfedge1 );


	private:
		Vertex( Number, Number );
		~Vertex( );
		Vertex* init( Number x, Number y );

		static std::vector< Vertex* > _pool;;
		static const Vertex* VERTEX_AT_INFINITY;
		static int _nvertices;
		Point* _coord;
		int _vertexIndex;


	};

} /* namespace Delaunay */
#endif /* VERTEX_H_ */
