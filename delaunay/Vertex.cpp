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
#include <math.h>

namespace Delaunay
{
	int Vertex::_nvertices = 0;
	Vertex* Vertex::VERTEX_AT_INFINITY = new Vertex( NAN, NAN );

	Vertex::Vertex( Number x, Number y )
	{
		init( x, y );
	}

	Vertex::~Vertex( )
	{
		// TODO Auto-generated destructor stub
	}

	Vertex* Vertex::init( Number x, Number y )
	{
		_coord = new Point( x, y );
		return this;
	}

	Vertex* Vertex::create( Number x, Number y )
	{
		if( isnan( x ) || isnan( y ) )
			return VERTEX_AT_INFINITY;
		Vertex* v = NULL;
		if( _pool.size( ) > 0 ){
			v = _pool.back( );
			_pool.pop_back( );
			v->init( x, y );
		}else{
			v = new Vertex( x, y );
		}
		return v;
	}

	void Vertex::dispose( )
	{
		delete _coord, _coord = NULL;
		_pool.push_back( this );
	}

	void Vertex::setIndex( )
	{
		_vertexIndex = _nvertices++;
	}

	Vertex* intersect( Halfedge* halfedge0, Halfedge* halfedge1 )
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
		if( -1.0e-10 < determinant && determinant < 1.0e-10 ) // the edges are parallel
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
		rightOfSite = intersectionX >= edge->rightSite()->x();
		if ((rightOfSite && halfedge->leftRight == LR::LEFT)
				||  (!rightOfSite && halfedge->leftRight == LR::RIGHT))
			return NULL;

		return Vertex::create(intersectionX, intersectionY);
	}

} /* namespace Delaunay */
