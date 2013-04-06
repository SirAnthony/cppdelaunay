/*
 * Edge.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "Edge.h"
#include "defines.h"
#include "math.h"
#ifdef DELAUNAY_DEBUG
	#include <stdio.h>
#endif


namespace Delaunay
{
	int Edge::_nedges = 0;
	Edge* Edge::DELETED = new Edge();
	std::list< Edge* > Edge::_pool;

	Edge::Edge( )
	{
		_edgeIndex = _nedges++;
		init( );
	}

	Edge::~Edge( )
	{
		_leftVertex = NULL;
		_rightVertex = NULL;
		if( _clippedVertices[LR::LEFT] ){
			_clippedVertices[LR::LEFT]->dispose();
			_clippedVertices[LR::LEFT] = NULL;
		}
		if( _clippedVertices[LR::RIGHT] ){
			_clippedVertices[LR::RIGHT]->dispose();
			_clippedVertices[LR::RIGHT] = NULL;
		}
		_sites[LR::LEFT] = NULL;
		_sites[LR::RIGHT] = NULL;
	}

	void Edge::clean( )
	{
		_pool.sort();
		_pool.unique();
		for( std::list< Edge* >::iterator it = _pool.begin( ), end = _pool.end( );
				it != end; ++it ){
			delete (*it);
		}
		_pool.clear();
	}

	LineSegment* Edge::delaunayLine( )
	{
		// draw a line connecting the input Sites for which the edge is a bisector:
		return LineSegment::create( leftSite( )->coord( ), rightSite( )->coord( ) );
	}

	LineSegment* Edge::voronoiEdge( )
	{
		if( !visible() )
			return LineSegment::create( NULL, NULL );
		return LineSegment::create( _clippedVertices[LR::LEFT], _clippedVertices[LR::RIGHT] );
	}

	Edge* Edge::createBisectingEdge( Site* site0, Site* site1 )
	{
		Number dx, dy, absdx, absdy;

		Edge* edge = Edge::create( );
		edge->leftSite( site0 );
		edge->rightSite( site1 );
		site0->addEdge( edge );
		site1->addEdge( edge );

		dx = site1->x() - site0->x();
		dy = site1->y() - site0->y();
		absdx = dx > 0 ? dx : -dx;
		absdy = dy > 0 ? dy : -dy;
		edge->c = site0->x() * dx + site0->y() * dy + (dx * dx + dy * dy) * 0.5;
		if( absdx > absdy ){
			edge->a = 1.0;
			edge->b = dy / dx;
			edge->c /= dx;
		}else{
			edge->b = 1.0;
			edge->a = dx / dy;
			edge->c /= dy;
		}

		edge->out();
		//trace("createBisectingEdge: a ", edge.a, "b", edge.b, "c", edge.c);

		return edge;
	}

	void Edge::dispose( )
	{
		_leftVertex = NULL;
		_rightVertex = NULL;
		if( _clippedVertices[LR::LEFT] ){
			_clippedVertices[LR::LEFT]->dispose();
			_clippedVertices[LR::LEFT] = NULL;
		}
		if( _clippedVertices[LR::RIGHT] ){
			_clippedVertices[LR::RIGHT]->dispose();
			_clippedVertices[LR::RIGHT] = NULL;
		}
		_sites[LR::LEFT] = NULL;
		_sites[LR::RIGHT] = NULL;

		_pool.push_back( this );
	}

	void Edge::setVertex( LR::Side leftRight, Vertex* v )
	{
		if( leftRight == LR::LEFT )
			_leftVertex = v;
		else
			_rightVertex = v;
		if( vertex( LR::other( leftRight ) ) )
			outEnd();
	}

	Number Edge::sitesDistance( )
	{
		return Point::distance( leftSite( )->coord( ), rightSite( )->coord( ) );
	}

	Number Edge::compareSitesDistances_MAX( Edge* edge0, Edge* edge1 )
	{
		Number length0 = edge0->sitesDistance( );
		Number length1 = edge1->sitesDistance( );
		if( length0 < length1 )
			return 1;
		if( length0 > length1 )
			return -1;
		return 0;
	}

	Number Edge::compareSitesDistances( Edge* edge0, Edge* edge1 )
	{
		return -compareSitesDistances_MAX( edge0, edge1 );
	}

	void Edge::clipVertices( const Rectangle& bounds )
	{
		Number xmin = bounds.x( );
		Number ymin = bounds.y( );
		Number xmax = bounds.right( );
		Number ymax = bounds.bottom( );

		Vertex* vertex0;
		Vertex* vertex1;
		Number x0, x1, y0, y1;

		if( a == 1.0 && b >= 0.0 ){
			vertex0 = _rightVertex;
			vertex1 = _leftVertex;
		}else{
			vertex0 = _leftVertex;
			vertex1 = _rightVertex;
		}

		if( a == 1.0 ){
			y0 = ymin;
			if( vertex0 != NULL && vertex0->y() > ymin )
				y0 = vertex0->y();
			if( y0 > ymax )
				return;
			x0 = c - b * y0;

			y1 = ymax;
			if( vertex1 != NULL && vertex1->y() < ymax )
				y1 = vertex1->y();
			if( y1 < ymin )
				return;
			x1 = c - b * y1;

			if( (x0 > xmax && x1 > xmax) || (x0 < xmin && x1 < xmin) )
				return;

			if( x0 > xmax ){
				x0 = xmax;
				y0 = (c - x0) / b;
			}else if( x0 < xmin ){
				x0 = xmin;
				y0 = (c - x0) / b;
			}

			if( x1 > xmax ){
				x1 = xmax;
				y1 = (c - x1) / b;
			}else if( x1 < xmin ){
				x1 = xmin;
				y1 = (c - x1) / b;
			}
		}else{
			x0 = xmin;
			if( vertex0 != NULL && vertex0->x() > xmin )
				x0 = vertex0->x();
			if( x0 > xmax )
				return;
			y0 = c - a * x0;

			x1 = xmax;
			if( vertex1 != NULL && vertex1->x() < xmax )
				x1 = vertex1->x();
			if( x1 < xmin )
				return;
			y1 = c - a * x1;

			if( (y0 > ymax && y1 > ymax) || (y0 < ymin && y1 < ymin) )
				return;

			if( y0 > ymax ){
				y0 = ymax;
				x0 = (c - y0) / a;
			}else if( y0 < ymin ){
				y0 = ymin;
				x0 = (c - y0) / a;
			}

			if( y1 > ymax ){
				y1 = ymax;
				x1 = (c - y1) / a;
			}else if( y1 < ymin ){
				y1 = ymin;
				x1 = (c - y1) / a;
			}
		}

		if( vertex0 == _leftVertex ){
			_clippedVertices[LR::LEFT] = Point::create( x0, y0 );
			_clippedVertices[LR::RIGHT] = Point::create( x1, y1 );
		}else{
			_clippedVertices[LR::RIGHT] = Point::create( x0, y0 );
			_clippedVertices[LR::LEFT] = Point::create( x1, y1 );
		}

#ifdef DELAUNAY_DEBUG
		printf( "cl %f %f %f %f\n", x0, y0, x1, y1 );
#endif
	}

	void Edge::out()
	{
#if DELAUNAY_DEBUG == 1
		printf("l %f %f %f\n", a, b, c);
#elif DELAUNAY_DEBUG > 1
		printf( "line(%d) %gx+%gy=%g, bisecting %d %d\n", _edgeIndex, a, b, c,
				leftSite()->index(), rightSite()->index() );
#endif
	}

	void Edge::outEnd()
	{
#ifdef DELAUNAY_DEBUG
		printf( "e %d", _edgeIndex );
		printf( " %d ", _leftVertex != NULL ? _leftVertex->vertexIndex() : -1 );
		printf( "%d\n", _rightVertex != NULL ? _rightVertex->vertexIndex() : -1 );
#endif
	}

	Edge* Edge::create( )
	{
		Edge* edge;
		if( _pool.size( ) > 0 ){
			edge = _pool.front( );
			_pool.pop_front( );
			edge->init( );
		}else{
			edge = new Edge( );
		}
		return edge;
	}

	void Edge::init( )
	{
		_leftVertex = NULL;
		_rightVertex = NULL;
		_clippedVertices[LR::LEFT] = _clippedVertices[LR::RIGHT] = NULL;
		_sites[LR::LEFT] = _sites[LR::RIGHT] = NULL;
	}

} /* namespace Delaunay */

