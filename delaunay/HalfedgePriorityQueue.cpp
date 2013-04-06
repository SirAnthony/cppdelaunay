/*
 * HalfedgePriorityQueue.cpp
 *
 *  Created on: 07.03.2013
 *
 */

#include "delaunay/HalfedgePriorityQueue.h"

namespace Delaunay
{

	HalfedgePriorityQueue::HalfedgePriorityQueue( Number ymin, Number deltay,
			int sqrt_nsites )
	{
		_ymin = ymin;
		_deltay = deltay;
		_hashsize = 4 * sqrt_nsites;
		initialize( );
	}

	HalfedgePriorityQueue::~HalfedgePriorityQueue( )
	{
		for( int i = 0; i < _hashsize; ++i ){
			_hash[i]->dispose( );
		}
	}

	void HalfedgePriorityQueue::insert( Halfedge* halfEdge )
	{
		Halfedge *previous, *next;
		int insertionBucket = bucket( halfEdge );
		if( insertionBucket < _minBucket )
			_minBucket = insertionBucket;
		previous = _hash[insertionBucket];
		while( (next = previous->nextInPriorityQueue) != NULL &&
				(halfEdge->ystar > next->ystar ||
				(halfEdge->ystar == next->ystar &&
				halfEdge->vertex->x( ) > next->vertex->x( ))) ){
			previous = next;
		}
		halfEdge->nextInPriorityQueue = previous->nextInPriorityQueue;
		previous->nextInPriorityQueue = halfEdge;
		_count++;
	}

	void HalfedgePriorityQueue::remove( Halfedge* halfEdge )
	{
		Halfedge* previous;
		int removalBucket = bucket( halfEdge );

		if( halfEdge->vertex != NULL ){
			previous = _hash[removalBucket];
			while( previous->nextInPriorityQueue != halfEdge ){
				previous = previous->nextInPriorityQueue;
			}
			previous->nextInPriorityQueue = halfEdge->nextInPriorityQueue;
			_count--;
			halfEdge->vertex = NULL;
			halfEdge->nextInPriorityQueue = NULL;
			halfEdge->dispose( );
		}
	}

	bool HalfedgePriorityQueue::empty( )
	{
		return _count == 0;
	}

	Point* HalfedgePriorityQueue::min( )
	{
		adjustMinBucket( );
		Halfedge* answer = _hash[_minBucket]->nextInPriorityQueue;
		return Point::create( answer->vertex->x( ), answer->ystar );
	}

	Halfedge* HalfedgePriorityQueue::extractMin()
	{
		// get the first real Halfedge in _minBucket
		Halfedge* answer = _hash[_minBucket]->nextInPriorityQueue;
		_hash[_minBucket]->nextInPriorityQueue = answer->nextInPriorityQueue;
		_count--;
		answer->nextInPriorityQueue = NULL;
		return answer;
	}

	void HalfedgePriorityQueue::initialize( )
	{
		_count = 0;
		_minBucket = 0;
		_hash.resize( _hashsize );
		// dummy Halfedge at the top of each hash
		for( int i = 0; i < _hashsize; ++i ){
			_hash[i] = Halfedge::createDummy( );
			_hash[i]->nextInPriorityQueue = NULL;
		}
	}

	int HalfedgePriorityQueue::bucket( Halfedge* halfEdge )
	{
		int theBucket = (halfEdge->ystar - _ymin) / _deltay * _hashsize;
		if( theBucket < 0 )
			theBucket = 0;
		if( theBucket >= _hashsize )
			theBucket = _hashsize - 1;
		return theBucket;
	}

	bool HalfedgePriorityQueue::isEmpty( int bucket )
	{
		return (_hash[bucket]->nextInPriorityQueue == NULL);
	}

	void HalfedgePriorityQueue::adjustMinBucket( )
	{
		while( _minBucket < _hashsize - 1 && isEmpty( _minBucket ) )
			++_minBucket;
	}

} /* namespace Delaunay */
