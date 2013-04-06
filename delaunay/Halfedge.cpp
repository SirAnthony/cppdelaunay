/*
 * Halfedge.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "Halfedge.h"

namespace Delaunay
{

	std::list< Halfedge* > Halfedge::_pool;


	Halfedge::Halfedge( Edge* edge, LR::Side lr )
	{
		init( edge, lr );
	}

	void Halfedge::clean( )
	{
		_pool.sort();
		_pool.unique( );
		for( std::list< Halfedge* >::iterator it = _pool.begin( ), end = _pool.end( );
				it != end; ++it ){
			delete (*it);
		}
		_pool.clear();
	}

	Halfedge* Halfedge::create( Edge* edge, LR::Side lr )
	{
		Halfedge* halfedge;
		if( _pool.size( ) > 0 ){
			halfedge = _pool.front( );
			_pool.pop_front( );
			halfedge->init( edge, lr );
		}else{
			halfedge = new Halfedge( edge, lr );
		}
		return halfedge;
	}

	void Halfedge::dispose( )
	{
		if( edgeListLeftNeighbor || edgeListRightNeighbor ) // still in EdgeList
			return;
		if( nextInPriorityQueue ) // still in PriorityQueue
			return;
		edge = NULL;
		leftRight = LR::LEFT;
		vertex = NULL;
		_pool.push_back( this );
	}

	void Halfedge::reallyDispose( )
	{
		edgeListLeftNeighbor = NULL;
		edgeListRightNeighbor = NULL;
		nextInPriorityQueue = NULL;
		edge = NULL;
		leftRight = LR::LEFT;
		vertex = NULL;
		_pool.push_back( this );
	}

	Halfedge* Halfedge::createDummy( )
	{
		return create( NULL, LR::LEFT );
	}

	Halfedge::~Halfedge( )
	{
		// TODO Auto-generated destructor stub
	}

	Halfedge* Halfedge::init( Edge* e, LR::Side lr )
	{
		edge = e;
		leftRight = lr;
		edgeListLeftNeighbor = NULL;
		edgeListRightNeighbor = NULL;
		nextInPriorityQueue = NULL;
		vertex = NULL;
		ystar = 0;
		return this;
	}

	bool Halfedge::isLeftOf( const Point* p )
	{
		Site* topSite;
		bool rightOfSite, above, fast;
		Number dxp, dyp, dxs, t1, t2, t3, yl;

		topSite = edge->rightSite( );
		rightOfSite = p->x > topSite->x( );
		if( rightOfSite && this->leftRight == LR::LEFT )
			return true;

		if( !rightOfSite && this->leftRight == LR::RIGHT )
			return false;

		if( edge->a == 1.0 ){
			dyp = p->y - topSite->y( );
			dxp = p->x - topSite->x( );
			fast = false;
			if( (!rightOfSite && edge->b < 0.0) || (rightOfSite && edge->b >= 0.0) ){
				above = dyp >= edge->b * dxp;
				fast = above;
			}else{
				above = ( ( p->x + p->y * edge->b ) > edge->c );
				if( edge->b < 0.0 )
					above = !above;
				if( !above )
					fast = true;
			}
			if( !fast ){
				dxs = topSite->x( ) - edge->leftSite( )->x( );
				above = ( edge->b * ( dxp * dxp - dyp * dyp ) )
						< ( dxs * dyp * ( 1.0 + 2.0 * dxp / dxs + edge->b * edge->b ) );
				if( edge->b < 0.0 )
					above = !above;
			}
		}else{  /* edge.b == 1.0 */
			yl = edge->c - edge->a * p->x;
			t1 = p->y - yl;
			t2 = p->x - topSite->x( );
			t3 = yl - topSite->y( );
			above = ( t1 * t1 ) > ( t2 * t2 + t3 * t3 );
		}
		return ( this->leftRight == LR::LEFT ? above : !above );
	}

} /* namespace Delaunay */
