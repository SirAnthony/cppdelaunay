/*
 * LineSegment.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "LineSegment.h"

namespace Delaunay
{
	std::list< LineSegment* > LineSegment::_pool;

	LineSegment::LineSegment( const Point* pt0, const Point* pt1 )
	{
		init( pt0, pt1 );
	}

	LineSegment::~LineSegment( )
	{
		// TODO Auto-generated destructor stub
	}

	void LineSegment::init( const Point* pt0, const Point* pt1 )
	{
		p0 = pt0;
		p1 = pt1;
	}

	LineSegment* LineSegment::create( const Point* pt0, const Point* pt1 )
	{
		LineSegment* l = NULL;
		if( _pool.size( ) > 0 ){
			l = _pool.front( );
			_pool.pop_front( );
			l->init( pt0, pt1 );
		}else{
			l = new LineSegment( pt0, pt1 );
		}
		return l;
	}

	void LineSegment::clean()
	{
		_pool.sort();
		_pool.unique();
		for( std::list< LineSegment* >::iterator it = _pool.begin( ), end = _pool.end( );
				it != end; ++it ){
			delete (*it);
		}
		_pool.clear();
	}

	void LineSegment::dispose()
	{
		_pool.push_back( this );
	}


	Number LineSegment::compareLengths_MAX( const LineSegment* segment0, const LineSegment* segment1 )
	{
		Number length0 = Point::distance( segment0->p0, segment0->p1 );
		Number length1 = Point::distance( segment1->p0, segment1->p1 );
		if( length0 < length1 )
			return 1;
		if( length0 > length1 )
			return -1;
		return 0;
	}

	Number LineSegment::compareLengths( const LineSegment* s0, const LineSegment* s1 )
	{
		return -compareLengths_MAX( s0, s1 );
	}

} /* namespace Delaunay */
