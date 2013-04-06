/*
 * EdgeList.cpp
 *
 *  Created on: 07.03.2013
 *
 */

#include "EdgeList.h"

namespace Delaunay
{

	EdgeList::EdgeList( Number xmin, Number deltax, int sqrt_nsites )
	{
		_xmin = xmin;
		_deltax = deltax;
		_hashsize = 2 * sqrt_nsites;

		_hash.resize( _hashsize );

		// two dummy Halfedges:
		_leftEnd = Halfedge::createDummy( );
		_rightEnd = Halfedge::createDummy( );
		_leftEnd->edgeListLeftNeighbor = NULL;
		_leftEnd->edgeListRightNeighbor = _rightEnd;
		_rightEnd->edgeListLeftNeighbor = _leftEnd;
		_rightEnd->edgeListRightNeighbor = NULL;
		_hash[0] = _leftEnd;
		_hash[_hashsize - 1] = _rightEnd;
	}

	EdgeList::~EdgeList( )
	{
		if( _leftEnd && _rightEnd )
			dispose();
	}

	void EdgeList::dispose( )
	{
		Halfedge* halfEdge = _leftEnd;
		Halfedge* prevHe = NULL;
		while( halfEdge != _rightEnd ){
			prevHe = halfEdge;
			halfEdge = halfEdge->edgeListRightNeighbor;
			prevHe->dispose( );
		}
		_leftEnd->reallyDispose();
		_leftEnd = NULL;
		_rightEnd->reallyDispose( );
		_rightEnd = NULL;
		_hash.clear( );
	}

	Halfedge* EdgeList::leftEnd( )
	{
		return _leftEnd;
	}

	Halfedge* EdgeList::rightEnd( )
	{
		return _rightEnd;
	}

	void EdgeList::insert( Halfedge* lb, Halfedge* newHalfedge )
	{
		newHalfedge->edgeListLeftNeighbor = lb;
		newHalfedge->edgeListRightNeighbor = lb->edgeListRightNeighbor;
		lb->edgeListRightNeighbor->edgeListLeftNeighbor = newHalfedge;
		lb->edgeListRightNeighbor = newHalfedge;
	}

	void EdgeList::remove( Halfedge* halfEdge )
	{
		halfEdge->edgeListLeftNeighbor->edgeListRightNeighbor =
				halfEdge->edgeListRightNeighbor;
		halfEdge->edgeListRightNeighbor->edgeListLeftNeighbor =
				halfEdge->edgeListLeftNeighbor;
		halfEdge->edge = Edge::DELETED;
		halfEdge->edgeListLeftNeighbor = halfEdge->edgeListRightNeighbor = NULL;
	}

	Halfedge* EdgeList::edgeListLeftNeighbor( const Point* p )
	{
		Halfedge* halfEdge = NULL;

		/* Use hash table to get close to desired halfedge */
		int bucket = (p->x - _xmin) / _deltax * _hashsize;
		if( bucket < 0 )
			bucket = 0;
		if( bucket >= _hashsize )
			bucket = _hashsize - 1;
		halfEdge = getHash( bucket );
		if( halfEdge == NULL ){
			for( int i = 1; true; ++i ){
				if( (halfEdge = getHash( bucket - i )) != NULL )
					break;
				if( (halfEdge = getHash( bucket + i )) != NULL )
					break;
			}
		}
		/* Now search linear list of halfedges for the correct one */
		if( halfEdge == leftEnd() || (halfEdge != rightEnd() && halfEdge->isLeftOf( p )) ){
			do{
				halfEdge = halfEdge->edgeListRightNeighbor;
			}while( halfEdge != rightEnd() && halfEdge->isLeftOf( p ) );
			halfEdge = halfEdge->edgeListLeftNeighbor;
		}else{
			do{
				halfEdge = halfEdge->edgeListLeftNeighbor;
			}while( halfEdge != leftEnd() && !halfEdge->isLeftOf( p ) );
		}

		/* Update hash table and reference counts */
		if( bucket > 0 && ( bucket < _hashsize - 1 ) )
			_hash[bucket] = halfEdge;

		return halfEdge;
	}

	Halfedge* EdgeList::getHash( int b )
	{
		if( b < 0 || b >= _hashsize )
			return NULL;

		Halfedge* halfEdge = _hash[b];
		if( halfEdge != NULL && halfEdge->edge == Edge::DELETED ){
			/* Hash table points to deleted halfedge.  Patch as necessary. */
			_hash[b] = NULL;
			// still can't dispose halfEdge yet!
			return NULL;
		}

		return halfEdge;
	}

} /* namespace Delaunay */
