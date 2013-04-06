/*
 * Voronoi.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "delaunay/Voronoi.h"
#include "delaunay/Functions.h"
#include "delaunay/EdgeReorderer.h"
#include "delaunay/EdgeList.h"
#include "delaunay/HalfedgePriorityQueue.h"
#include <stdlib.h>
#include <cmath>

namespace Delaunay
{
	int Voronoi::_instances = 0;


	Voronoi::Voronoi( const std::vector< Point* >& points,
			const std::vector< unsigned >* colors, Rectangle& plotBounds ) :
			_plotBounds( plotBounds ), _sites( NULL )
	{
		_instances++;
		addSites( points, colors );
		fortunesAlgorithm( );
	}

	Voronoi::~Voronoi( )
	{
		_sitesIndexedByLocation.clear( );
		delete _sites;
		int n = _edges.size( );
		for( int i = 0; i < n; ++i ){
			_edges[i]->dispose( );
		}
		_instances--;
		clean();
	}

	void Voronoi::clean()
	{
		if( _instances <=0 ){
			Halfedge::clean();
			Edge::clean();
			Site::clean();
			Vertex::clean();
			LineSegment::clean();
			Point::clean();
		}
	}

	const Rectangle& Voronoi::plotBounds( )
	{
		return _plotBounds;
	}

	const std::vector< Edge* >& Voronoi::edges( )
	{
		return _edges;
	}

	std::vector< Point* > Voronoi::region( Point* p )
	{
		Site* site = _sitesIndexedByLocation[p];
		if( !site )
			return std::vector< Point* >( );
		return site->region( _plotBounds );
	}

	std::vector< const Point* > Voronoi::neighborSitesForSite( Point* coord )
	{
		std::vector< const Point* > points;
		Site* site = _sitesIndexedByLocation[coord];
		if( !site )
			return points;
		std::vector< Site* > sites = site->neighborSites( );
		for( std::vector< Site* >::iterator it = sites.begin( ); it != sites.end( );
				++it ){
			points.push_back( (*it)->coord( ) );
		}
		return points;
	}

	std::vector< LineSegment* > Voronoi::voronoiBoundaryForSite( Point* coord )
	{
		std::vector< Edge* > r = selectEdgesForSitePoint( coord, _edges );
		return visibleLineSegments( r );
	}

	std::vector< LineSegment* > Voronoi::delaunayLinesForSite( Point* coord )
	{
		std::vector< Edge* > r = selectEdgesForSitePoint( coord, _edges );
		return delaunayLinesForEdges( r );
	}

	std::vector< LineSegment* > Voronoi::voronoiDiagram( )
	{
		return visibleLineSegments( _edges );
	}

	std::vector< LineSegment* > Voronoi::delaunayTriangulation( /* keepOutMask:BitmapData = null */)
	{
		std::vector< Edge* > r = selectNonIntersectingEdges( /*keepOutMask,*/_edges );
		return delaunayLinesForEdges( r );
	}

	std::vector< LineSegment* > Voronoi::hull( )
	{
		std::vector< Edge* > r = hullEdges( );
		return delaunayLinesForEdges( r );
	}

	std::vector< const Point* > Voronoi::hullPointsInOrder( )
	{
		std::vector< Edge* > hEdges = hullEdges( );
		std::vector< const Point* > points;
		if( hEdges.size( ) == 0 )
			return points;

		EdgeReorderer reorderer( hEdges, EdgeReorderer::cSite );
		hEdges = reorderer.edges( );
		std::vector< LR::Side > orientations = reorderer.edgeOrientations( );
		LR::Side orientation;
		int n = hEdges.size( );
		for( int i = 0; i < n; ++i ){
			Edge* edge = hEdges[i];
			orientation = orientations[i];
			points.push_back( edge->site( orientation )->coord( ) );
		}
		return points;
	}

	std::vector< LineSegment* > Voronoi::spanningTree(
			enum KruskalType type /*, keepOutMask:BitmapData = null */)
	{
		std::vector< Edge* > edges = selectNonIntersectingEdges( /*keepOutMask,*/_edges );
		std::vector< LineSegment* > segments = delaunayLinesForEdges( edges );
		return kruskal( segments, type );
	}

	std::vector< std::vector< Point* > > Voronoi::regions( )
	{
		return _sites->regions( _plotBounds );
	}

	void Voronoi::regionsPrepare( )
	{
		_sites->regionsPrepare( _plotBounds );
	}

	std::vector< unsigned > Voronoi::siteColors( /* referenceImage:BitmapData = null */)
	{
		return _sites->siteColors( /* referenceImage */);
	}

	const Point* Voronoi::nearestSitePoint( /* proximityMap:BitmapData,*/Number x,
			Number y )
	{
		return _sites->nearestSitePoint( /*proximityMap,*/x, y );
	}

	std::vector< const Point* > Voronoi::siteCoords( )
	{
		return _sites->siteCoords( );
	}

	template<>
	Number Voronoi::compareByYThenX( const Site* s1, Site* s2 )
	{
		if( s1->y( ) < s2->y( ) )
			return -1;
		if( s1->y( ) > s2->y( ) )
			return 1;
		if( s1->x( ) < s2->x( ) )
			return -1;
		if( s1->x( ) > s2->x( ) )
			return 1;
		return 0;
	}

	template<>
	Number Voronoi::compareByYThenX( const Site* s1, Point* s2 )
	{
		if( s1->y( ) < s2->y )
			return -1;
		if( s1->y( ) > s2->y )
			return 1;
		if( s1->x( ) < s2->x )
			return -1;
		if( s1->x( ) > s2->x )
			return 1;
		return 0;
	}

	void Voronoi::addSites( const std::vector< Point* >& points,
			const std::vector< unsigned >* colors )
	{
		if( !_sites )
			_sites = new SiteList();

		unsigned length = points.size( );
		for( unsigned i = 0; i < length; ++i ){
			addSite( points[i], colors ? colors->at( i ) : 0, i );
		}
	}

	void Voronoi::addSite( Point* p, unsigned color, int index )
	{
		Number weight = random( ) * 100;
		Site* site = Site::create( p, index, weight, color );
		_sites->push(site);
		_sitesIndexedByLocation[p] = site;
	}

	std::vector< Edge* > Voronoi::hullEdges( )
	{
		std::vector< Edge* > n;
		for( std::vector< Edge* >::iterator it = _edges.begin( ); it < _edges.end( );
				++it ){
			Edge* edge = (*it);
			if( edge->isPartOfConvexHull( ) )
				n.push_back( edge );
		}

		return n;
	}

	inline Site* leftRegion( Halfedge* he, Site* bottomMostSite )
	{
		Edge* edge = he->edge;
		if( edge == NULL )
			return bottomMostSite;
		return edge->site( he->leftRight );
	}

	inline Site* rightRegion( Halfedge* he, Site* bottomMostSite )
	{
		Edge* edge = he->edge;
		if( edge == NULL )
			return bottomMostSite;
		return edge->site( LR::other( he->leftRight ) );
	}

	void Voronoi::fortunesAlgorithm( )
	{
		Site *newSite, *bottomSite, *topSite, *tempSite;
		Vertex *v, *vertex = NULL;
		Point* newintstar = NULL;
		LR::Side leftRight;
		Halfedge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
		Edge* edge;

		const Rectangle& dataBounds = _sites->sitesBounds( );

		int sqrt_nsites = (int) sqrt( _sites->length( ) + 4 );
		HalfedgePriorityQueue heap( dataBounds.y( ), dataBounds.height( ), sqrt_nsites );
		EdgeList edgeList( dataBounds.x( ), dataBounds.width( ), sqrt_nsites );
		std::vector< Halfedge* > halfEdges;
		std::vector< Vertex* > vertices;

		Site* bottomMostSite = _sites->next( );
		newSite = _sites->next( );

		while( true ){
			if( heap.empty( ) == false )
				newintstar = heap.min( );

			if( newSite != NULL
					&& (heap.empty( ) || compareByYThenX( newSite, newintstar ) < 0) ){
				// new site is smallest
				newSite->out(); //trace("smallest: new site " + newSite);

				// Step 8:
				lbnd = edgeList.edgeListLeftNeighbor( newSite->coord( ) );// the Halfedge just to the left of newSite
				//trace("lbnd: " + lbnd);
				rbnd = lbnd->edgeListRightNeighbor;		// the Halfedge just to the right
				//trace("rbnd: " + rbnd);
				bottomSite = rightRegion( lbnd, bottomMostSite );// this is the same as leftRegion(rbnd)
				// this Site determines the region containing the new site
				//trace("new Site is in region of existing site: " + bottomSite);

				// Step 9:
				edge = Edge::createBisectingEdge( bottomSite, newSite );
				//trace("new edge: " + edge);
				_edges.push_back( edge );

				bisector = Halfedge::create( edge, LR::LEFT );
				halfEdges.push_back( bisector );
				// inserting two Halfedges into edgeList constitutes Step 10:
				// insert bisector to the right of lbnd:
				edgeList.insert( lbnd, bisector );

				// first half of Step 11:
				if( (vertex = Vertex::intersect( lbnd, bisector )) != NULL ){
					vertices.push_back( vertex );
					heap.remove( lbnd );
					lbnd->vertex = vertex;
					lbnd->ystar = vertex->y( ) + newSite->dist( vertex );
					heap.insert( lbnd );
				}

				lbnd = bisector;
				bisector = Halfedge::create( edge, LR::RIGHT );
				halfEdges.push_back( bisector );
				// second Halfedge for Step 10:
				// insert bisector to the right of lbnd:
				edgeList.insert( lbnd, bisector );

				// second half of Step 11:
				if( (vertex = Vertex::intersect( bisector, rbnd )) != NULL ){
					vertices.push_back( vertex );
					bisector->vertex = vertex;
					bisector->ystar = vertex->y( ) + newSite->dist( vertex );
					heap.insert( bisector );
				}

				newSite = _sites->next( );
			}else if( heap.empty( ) == false ){
				//intersection is smallest
				lbnd = heap.extractMin( );
				llbnd = lbnd->edgeListLeftNeighbor;
				rbnd = lbnd->edgeListRightNeighbor;
				rrbnd = rbnd->edgeListRightNeighbor;
				bottomSite = leftRegion( lbnd, bottomMostSite );
				topSite = rightRegion( rbnd, bottomMostSite );
				// these three sites define a Delaunay triangle
				// (not actually using these for anything...)
				//_triangles.push(new Triangle(bottomSite, topSite, rightRegion(lbnd)));

				v = lbnd->vertex;
				v->setIndex( );
				lbnd->edge->setVertex( lbnd->leftRight, v );
				rbnd->edge->setVertex( rbnd->leftRight, v );
				edgeList.remove( lbnd );
				heap.remove( rbnd );
				edgeList.remove( rbnd );
				leftRight = LR::LEFT;
				if( bottomSite->y( ) > topSite->y( ) ){
					tempSite = bottomSite;
					bottomSite = topSite;
					topSite = tempSite;
					leftRight = LR::RIGHT;
				}
				edge = Edge::createBisectingEdge( bottomSite, topSite );
				_edges.push_back( edge );
				bisector = Halfedge::create( edge, leftRight );
				halfEdges.push_back( bisector );
				edgeList.insert( llbnd, bisector );
				edge->setVertex( LR::other( leftRight ), v );
				if( (vertex = Vertex::intersect( llbnd, bisector )) != NULL ){
					vertices.push_back( vertex );
					heap.remove( llbnd );
					llbnd->vertex = vertex;
					llbnd->ystar = vertex->y( ) + bottomSite->dist( vertex );
					heap.insert( llbnd );
				}
				if( (vertex = Vertex::intersect( bisector, rrbnd )) != NULL ){
					vertices.push_back( vertex );
					bisector->vertex = vertex;
					bisector->ystar = vertex->y( ) + bottomSite->dist( vertex );
					heap.insert( bisector );
				}
			}else{
				break;
			}

			if( newintstar ){
				newintstar->dispose();
				newintstar = NULL;
			}
		}

		edgeList.dispose();

		// heap should be empty now
		for( std::vector< Halfedge* >::iterator it = halfEdges.begin( );
				it != halfEdges.end( ); ++it )
			(*it)->reallyDispose( );
		halfEdges.clear( );

		// we need the vertices to clip the edges
		for( std::vector< Edge* >::iterator it = _edges.begin( ); it != _edges.end( );
				++it )
			(*it)->clipVertices( _plotBounds );

		// but we don't actually ever use them again!
		for( std::vector< Vertex* >::iterator it = vertices.begin( );
				it != vertices.end( ); ++it )
			(*it)->dispose( );
		vertices.clear( );
	}
} /* namespace Delaunay */
