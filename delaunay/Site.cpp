/*
 * Site.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "delaunay/Site.h"
#include "delaunay/Edge.h"
#include "delaunay/Voronoi.h"
#include "delaunay/EdgeReorderer.h"
#include "geom/Polygon.h"
#include <algorithm>
#include <vector>

#ifdef DELAUNAY_DEBUG
	#include <stdio.h>
#endif

#define EPSILON .005


namespace Delaunay
{
	std::list< Site* > Site::_pool;
	int Site::_count = 0;

	int BoundsCheck::check( const Point* point, const Rectangle& bounds )
	{
		int value = 0;
		if( point->x == bounds.left() ){
			value |= LEFT;
		}
		if( point->x == bounds.right() ){
			value |= RIGHT;
		}
		if( point->y == bounds.top() ){
			value |= TOP;
		}
		if( point->y == bounds.bottom() ){
			value |= BOTTOM;
		}
		return value;
	}


	Site::Site( Point* p, int index, Number weight, unsigned color )
	{
		_count++;
		init( p, index, weight, color );
	}

	Site::~Site( )
	{
		_count--;
	}

	void Site::clean( )
	{
		_pool.sort();
		_pool.unique();
		for( std::list< Site* >::iterator it = _pool.begin( ), end = _pool.end( );
				it != end; ++it ){
			delete (*it);
		}
		_pool.clear();
	}

	Site* Site::create( Point* p, int index, Number weight, unsigned color )
	{
		Site* site;
		if( _pool.size( ) > 0 ){
			site = _pool.front( );
			_pool.pop_front( );
			site->init( p, index, weight, color );
		}else{
			site = new Site( p, index, weight, color );
		}
		return site;
	}

	void Site::dispose( )
	{
		_coord = NULL;
		clear( );
		_pool.push_back( this );
	}

	Site* Site::init( Point* p, int index, Number w, unsigned c )
	{
		_coord = p;
		_siteIndex = index;
		weight = w;
		_color = c;
		clear( );
		return this;
	}

	void Site::sortSites( std::vector< Site* >& sites )
	{
		std::sort( sites.begin( ), sites.end( ), Site::compare );
	}

	Edge* Site::nearestEdge( )
	{
		std::sort( _edges.begin( ), _edges.end( ), Edge::compareSitesDistances );
		return _edges[0];
	}

	std::vector< Site* > Site::neighborSites( )
	{
		if( _edges.size( ) == 0 ){
			return std::vector< Site* >();
		}
		if( !_edgeReordered )
			reorderEdges( );

		std::vector< Site* > list;
		for( std::vector< Edge* >::iterator it = _edges.begin( ); it != _edges.end( ); ++it ){
			list.push_back( neighborSite( *it ) );
		}
		return list;
	}

	void Site::regionPrepare( const Rectangle& clippingBounds )
	{
		if( _edgeReordered )
			return;

		reorderEdges( );
		_region = clipToBounds( clippingBounds );
		if( Polygon( _region ).winding( ) == Winding::CLOCKWISE )
			std::reverse( _region.begin(), _region.end() );
	}

	std::vector< Point* > Site::region( const Rectangle& clippingBounds )
	{
		if( _edges.size( ) == 0 )
			return std::vector< Point* >();
		regionPrepare( clippingBounds );
		return _region;
	}

	void Site::out( )
	{
#if DELAUNAY_DEBUG == 1
		printf("s %f %f\n", _coord->x, _coord->y);
#elif DELAUNAY_DEBUG > 1
		printf("site (%d) at %f %f\n", _siteIndex, _coord->x, _coord->y);
#endif
	}


	int Site::compareInt( Site* s1, Site* s2 )
	{
		int returnValue = Voronoi::compareByYThenX( s1, s2 );

		// swap _siteIndex values if necessary to match new ordering:
		int tempIndex;
		if( returnValue == -1 ){
			if( s1->_siteIndex > s2->_siteIndex ){
				tempIndex = s1->_siteIndex;
				s1->_siteIndex = s2->_siteIndex;
				s2->_siteIndex = tempIndex;
			}
		}else if( returnValue == 1 ){
			if( s2->_siteIndex > s1->_siteIndex ){
				tempIndex = s2->_siteIndex;
				s2->_siteIndex = s1->_siteIndex;
				s1->_siteIndex = tempIndex;
			}

		}
		return returnValue;
	}

	bool Site::compare( Site* s1, Site* s2 )
	{
		return (compareInt( s1, s2 ) < 0);
	}

	bool Site::closeEnough( const Point* p0, const Point* p1 )
	{
		return Point::distance( p0, p1 ) < EPSILON;
	}

	void Site::move( Point* p )
	{
		clear( );
		_coord = p;
	}

	void Site::clear( )
	{
		_edges.clear( );
		_edgeOrientations.clear( );
		_region.clear( );
		_edgeReordered = false;
	}

	Site* Site::neighborSite( Edge* edge )
	{
		if( this == edge->leftSite() ){
			return edge->rightSite();
		}
		if( this == edge->rightSite() ){
			return edge->leftSite();
		}
		return NULL;
	}

	void Site::reorderEdges( )
	{
		EdgeReorderer reorderer( _edges, EdgeReorderer::cVertex );
		_edges = reorderer.edges();
		_edgeOrientations = reorderer.edgeOrientations();
		_edgeReordered = true;
	}

	std::vector< Point* > Site::clipToBounds( const Rectangle& bounds )
	{
		std::vector< Point* > points;
		int n = _edges.size( );
		int i = -1;
		for( int j = 0; j < n; ++j ){
			Edge* edge = _edges[j];
			if( !edge || !edge->visible() )
				continue;
			if( i >= 0 ){
				connect( points, j, bounds );
			}else{
				i = j;
				LR::Side orientation = _edgeOrientations[j];
				points.push_back( edge->clippedEnds()[orientation] );
				points.push_back( edge->clippedEnds()[LR::other( orientation )] );
			}
		}

		// close up the polygon by adding another corner point of the bounds if needed:
		if( i >= 0 )
			connect( points, i, bounds, true );

		return points;
	}

	void Site::connect( std::vector< Point* >& points, int j, const Rectangle& bounds,
			bool closingUp )
	{
		Point* rightPoint = points[points.size( ) - 1];
		Edge* newEdge = _edges[j];
		LR::Side newOrientation = _edgeOrientations[j];
		// the point that  must be connected to rightPoint:
		Point* newPoint = newEdge->clippedEnds()[newOrientation];
		if( !closeEnough( rightPoint, newPoint ) ){
			// The points do not coincide, so they must have been clipped at the bounds;
			// see if they are on the same border of the bounds:
			if( rightPoint->x != newPoint->x && rightPoint->y != newPoint->y ){
				// They are on different borders of the bounds;
				// insert one or two corners of bounds as needed to hook them up:
				// (NOTE this will not be correct if the region should take up more than
				// half of the bounds rect, for then we will have gone the wrong way
				// around the bounds and included the smaller part rather than the larger)
				int rightCheck = BoundsCheck::check( rightPoint, bounds );
				int newCheck = BoundsCheck::check( newPoint, bounds );
				Number px, py;

				// TODO: refactor origin lib copypasta
				if( rightCheck & BoundsCheck::RIGHT ){
					px = bounds.right( );
					if( newCheck & BoundsCheck::BOTTOM ){
						py = bounds.bottom( );
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::TOP ){
						py = bounds.top( );
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::LEFT ){
						if( rightPoint->y - bounds.y( ) + newPoint->y - bounds.y( )
								< bounds.height( ) ){
							py = bounds.top( );
						}else{
							py = bounds.bottom( );
						}
						points.push_back( Point::create( px, py ) );
						points.push_back( Point::create( bounds.left( ), py ) );
					}
				}else if( rightCheck & BoundsCheck::LEFT ){
					px = bounds.left();
					if( newCheck & BoundsCheck::BOTTOM ){
						py = bounds.bottom();
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::TOP ){
						py = bounds.top();
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::RIGHT ){
						if( rightPoint->y - bounds.y() + newPoint->y - bounds.y()
								< bounds.height() ){
							py = bounds.top();
						}else{
							py = bounds.bottom();
						}
						points.push_back( Point::create( px, py ) );
						points.push_back( Point::create( bounds.right(), py ) );
					}
				}else if( rightCheck & BoundsCheck::TOP ){
					py = bounds.top();
					if( newCheck & BoundsCheck::RIGHT ){
						px = bounds.right();
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::LEFT ){
						px = bounds.left();
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::BOTTOM ){
						if( rightPoint->x - bounds.x() + newPoint->x - bounds.x()
								< bounds.width() ){
							px = bounds.left();
						}else{
							px = bounds.right();
						}
						points.push_back( Point::create( px, py ) );
						points.push_back( Point::create( px, bounds.bottom() ) );
					}
				}else if( rightCheck & BoundsCheck::BOTTOM ){
					py = bounds.bottom();
					if( newCheck & BoundsCheck::RIGHT ){
						px = bounds.right();
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::LEFT ){
						px = bounds.left();
						points.push_back( Point::create( px, py ) );
					}else if( newCheck & BoundsCheck::TOP ){
						if( rightPoint->x - bounds.x() + newPoint->x - bounds.x()
								< bounds.width() ){
							px = bounds.left();
						}else{
							px = bounds.right();
						}
						points.push_back( Point::create( px, py ) );
						points.push_back( Point::create( px, bounds.top() ) );
					}
				}
			}
			if( closingUp ){
				// newEdge's ends have already been added
				return;
			}
			points.push_back( newPoint );
		}
		Point* newRightPoint = newEdge->clippedEnds()[LR::other( newOrientation )];
		if( !closeEnough( points[0], newRightPoint ) ){
			points.push_back( newRightPoint );
		}
	}

} /* namespace Delaunay */
