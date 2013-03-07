/*
 * Edge.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "Edge.h"
#include "defines.h"

namespace Delaunay
{
	int Edge::_nedges = 0;
	Edge* Edge::DELETED = new Edge();
	std::vector< Edge* > Edge::_pool;

	Edge::Edge( )
	{
		_edgeIndex = _nedges++;
		init( );
	}

	Edge::~Edge( )
	{
		dispose( );
	}

	LineSegment* Edge::delaunayLine( )
	{
		// draw a line connecting the input Sites for which the edge is a bisector:
		return new LineSegment( leftSite( )->coord( ), rightSite( )->coord( ) );
	}

	LineSegment* Edge::voronoiEdge( )
	{
		if( !visible() )
			return new LineSegment( NULL, NULL );
		return new LineSegment( _clippedVertices[LR::LEFT], _clippedVertices[LR::RIGHT] );
	}

	Edge* Edge::createBisectingEdge( Site* site0, Site* site1 )
	{
		Number dx, dy, absdx, absdy;
		Number a, b, c;

		dx = site1->x() - site0->x();
		dy = site1->y() - site0->y();
		absdx = dx > 0 ? dx : -dx;
		absdy = dy > 0 ? dy : -dy;
		c = site0->x() * dx + site0->y() * dy + (dx * dx + dy * dy) * 0.5;
		if( absdx > absdy ){
			a = 1.0;
			b = dy / dx;
			c /= dx;
		}else{
			b = 1.0;
			a = dx / dy;
			c /= dy;
		}

		Edge* edge = Edge::create( );
		edge->leftSite( site0 );
		edge->rightSite( site1 );
		site0->addEdge( edge );
		site1->addEdge( edge );

		edge->_leftVertex = NULL;
		edge->_rightVertex = NULL;

		edge->a = a;
		edge->b = b;
		edge->c = c;
		//trace("createBisectingEdge: a ", edge.a, "b", edge.b, "c", edge.c);

		return edge;
	}

	void Edge::dispose( )
	{
		_leftVertex = NULL;
		_rightVertex = NULL;
		if( _clippedVertices[LR::LEFT] ){
			delete _clippedVertices[LR::LEFT], _clippedVertices[LR::LEFT] = NULL;
		}
		if( _clippedVertices[LR::RIGHT] ){
			delete _clippedVertices[LR::RIGHT], _clippedVertices[LR::RIGHT] = NULL;
		}
		_sites[LR::LEFT] = NULL;
		_sites[LR::RIGHT] = NULL;

		_pool.push_back( this );
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
			_clippedVertices[LR::LEFT] = new Point( x0, y0 );
			_clippedVertices[LR::RIGHT] = new Point( x1, y1 );
		}else{
			_clippedVertices[LR::RIGHT] = new Point( x0, y0 );
			_clippedVertices[LR::LEFT] = new Point( x1, y1 );
		}
	}

	Edge* Edge::create( )
	{
		Edge* edge;
		if( _pool.size( ) > 0 ){
			edge = _pool.back( );
			_pool.pop_back( );
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

