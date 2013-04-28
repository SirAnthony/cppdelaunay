/*
 * Vertex.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "delaunay/Vertex.h"
#include "delaunay/Edge.h"
#include "delaunay/Halfedge.h"
#include "delaunay/Voronoi.h"
#include "math.h"

#ifdef DELAUNAY_DEBUG
	#include <stdio.h>
#endif

namespace Delaunay
{
	int Vertex::_nvertices = 0;
	Vertex* Vertex::VERTEX_AT_INFINITY = NULL;
	std::list< Vertex* > Vertex::_pool;

	Vertex::Vertex( Number x, Number y )
	{
		init( x, y );
	}

	Vertex::~Vertex( )
	{
		if( _coord )
			_coord->dispose();
	}


	void Vertex::clean( )
	{
		if( VERTEX_AT_INFINITY ){
			VERTEX_AT_INFINITY->dispose();
			VERTEX_AT_INFINITY = NULL;
		}

		_pool.sort();
		_pool.unique( );
		for( std::list< Vertex* >::iterator it = _pool.begin( ), end = _pool.end( );
				it != end; ++it ){
			delete (*it);
		}
		_pool.clear();
	}

	Vertex* Vertex::init( Number x, Number y )
	{
		if( _coord )
			_coord->dispose();
		_coord = Point::create( x, y );
		return this;
	}

	Vertex* Vertex::create( Number x, Number y )
	{
		if( VERTEX_AT_INFINITY == NULL )
			VERTEX_AT_INFINITY = new Vertex( NAN, NAN );

		if( isnan( x ) || isnan( y ) )
			return VERTEX_AT_INFINITY;
		Vertex* v = NULL;
		if( _pool.size( ) > 0 ){
			v = _pool.front( );
			_pool.pop_front( );
			v->init( x, y );
		}else{
			v = new Vertex( x, y );
		}
		return v;
	}

	void Vertex::dispose( )
	{
		if( _coord ){
			_coord->dispose();
			_coord = NULL;
		}
		_pool.push_back( this );
	}

	void Vertex::setIndex( )
	{
		_vertexIndex = _nvertices++;
		out();
	}

	void Vertex::out( )
	{
#if DELAUNAY_DEBUG == 1
	printf("v %f %f\n", _coord->x, _coord->y);
#elif DELAUNAY_DEBUG > 1
	printf("vertex(%d) at %f %f\n", _vertexIndex, _coord->x, _coord->y);
#endif
	}

	Vertex* Vertex::intersect( Halfedge* halfedge0, Halfedge* halfedge1 )
	{
		Edge* edge0;
		Edge* edge1;
		Edge* edge;
		Halfedge* halfedge;
		Number determinant, intersectionX, intersectionY;
		bool rightOfSite;

		edge0 = halfedge0->edge;
		edge1 = halfedge1->edge;
		if( edge0 == NULL || edge1 == NULL )
			return NULL;
		if (edge0->rightSite() == edge1->rightSite())
			return NULL;

		determinant = edge0->a * edge1->b - edge0->b * edge1->a;
		if( ( -1.0e-10 < determinant ) && ( determinant < 1.0e-10 ) ) // the edges are parallel
			return NULL;

		intersectionX = (edge0->c * edge1->b - edge1->c * edge0->b)/determinant;
		intersectionY = (edge1->c * edge0->a - edge0->c * edge1->a)/determinant;

		if( Voronoi::compareByYThenX(edge0->rightSite(), edge1->rightSite()) < 0){
			halfedge = halfedge0;
			edge = edge0;
		}else{
			halfedge = halfedge1;
			edge = edge1;
		}
		rightOfSite = ( intersectionX >= edge->rightSite()->x() );
		if ((rightOfSite && halfedge->leftRight == LR::LEFT)
				||  (!rightOfSite && halfedge->leftRight == LR::RIGHT))
			return NULL;

		return Vertex::create(intersectionX, intersectionY);
	}

} /* namespace Delaunay */
