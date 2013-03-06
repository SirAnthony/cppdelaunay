/*
 * Voronoi.cpp
 *
 *  Created on: 06.03.2013
 *
 */

#include "delaunay/Voronoi.h"
#include "delaunay/Functions.h"

namespace Delaunay {

	Voronoi::Voronoi( const std::vector<Point*>& points,
			const std::vector<unsigned>* colors, Rectangle& plotBounds )
	{
		addSites( points, colors );
		_plotBounds = plotBounds;
		fortunesAlgorithm();
	}

	Voronoi::~Voronoi( )
	{
		int n = _edges.size();
		for( int i = 0; i < n; ++i ){
			_edges[i]->dispose();
		}
		_sitesIndexedByLocation.clear();
	}

	const Rectangle& Voronoi::plotBounds( )
	{
		return _plotBounds;
	}

	const std::vector<Edge*>& Voronoi::edges( )
	{
		return _edges;
	}

	std::vector<Point*> Voronoi::region( Point* p )
	{
		Site* site = _sitesIndexedByLocation[p];
		if( !site )
			return std::vector<Point*>();
		return site->region( _plotBounds );
	}

	std::vector<const Point*> Voronoi::neighborSitesForSite( Point* coord )
	{
		std::vector<const Point*> points;
		Site* site = _sitesIndexedByLocation[coord];
		if( !site )
			return points;
		std::vector<Site*> sites = site->neighborSites();
		for( std::vector<Site*>::iterator it = sites.begin(); it != sites.end(); ++it ){
			points.push_back( ( *it )->coord() );
		}
		return points;
	}

	std::vector<LineSegment*> Voronoi::voronoiBoundaryForSite( Point* coord )
	{
		return visibleLineSegments( selectEdgesForSitePoint( coord, _edges ) );
	}

	std::vector<LineSegment*> Voronoi::delaunayLinesForSite( Point* coord )
	{
		return delaunayLinesForEdges( selectEdgesForSitePoint( coord, _edges ) );
	}

	std::vector<LineSegment*> Voronoi::voronoiDiagram( )
	{
		return visibleLineSegments( _edges );
	}

	std::vector<LineSegment*> Voronoi::delaunayTriangulation( /* keepOutMask:BitmapData = null */)
	{
		return delaunayLinesForEdges(
				selectNonIntersectingEdges( /*keepOutMask,*/_edges ) );
	}

	std::vector<LineSegment*> Voronoi::hull( )
	{
		return delaunayLinesForEdges( hullEdges() );
	}

	std::vector<const Point*> Voronoi::hullPointsInOrder( )
	{
		std::vector<Edge*> hullEdges = hullEdges();
		std::vector<const Point*> points;
		if( hullEdges.size() == 0 )
			return points;

		EdgeReorderer reorderer( hullEdges, EdgeReorderer::cSite );
		hullEdges = reorderer.edges();
		std::vector<LR::Side> orientations = reorderer.edgeOrientations();
		LR::Side orientation;
		int n = hullEdges.size();
		for( int i = 0; i < n; ++i ){
			Edge* edge = hullEdges[i];
			orientation = orientations[i];
			points.push_back( edge->site( orientation )->coord() );
		}
		return points;
	}

	std::vector<LineSegment*> Voronoi::spanningTree(
			int type /* = "minimum" , keepOutMask:BitmapData = null */)
	{
		std::vector<Edge*> = selectNonIntersectingEdges( /*keepOutMask,*/_edges );
		std::vector<LineSegment*> segments = delaunayLinesForEdges( edges );
		return std::vector<LineSegment*>(); // kruskal(segments, type);
	}

	std::vector<const Point*> Voronoi::regions( )
	{
		return _sites.regions( _plotBounds );
	}

	std::vector<unsigned> Voronoi::siteColors( /* referenceImage:BitmapData = null */)
	{
		return _sites.siteColors( referenceImage );
	}

	Point* Voronoi::nearestSitePoint( /* proximityMap:BitmapData,*/ Number x, Number y )
	{
		return _sites.nearestSitePoint( /*proximityMap,*/ x, y );
	}

	std::vector<const Point*> siteCoords( )
	{
		return _sites.siteCoords();
	}

	static Number compareByYThenX( const Site* s1, const Site* s2 )
	{
		if (s1->y < s2->y) return -1;
		if (s1->y > s2->y) return 1;
		if (s1->x < s2->x) return -1;
		if (s1->x > s2->x) return 1;
		return 0;
	}

	void Voronoi::addSites( const std::vector<Point*>& points,
			const std::vector<unsigned>* colors )
	{
		unsigned length = points.size();
		for( unsigned i = 0; i < length; ++i ){
			addSite( points[i], colors ? colors->at( i ) : 0, i );
		}
	}

	std::vector<Edge*> Voronoi::hullEdges( )
	{
		std::vector<Edge*> n;
		for( std::vector<Edge*>::iterator it = _edges.begin(); it < _edges.end(); ++it ){
			Edge* edge = ( *it );
			if( edge->isPartOfConvexHull() )
				n.push_back( edge );
		}

		return n;
	}

	void Voronoi::fortunesAlgorithm()
	{
		Site *newSite, *bottomSite, *topSite, *tempSite;
		Vertex *v, *vertex;
		Point* newintstar;
		LR::Side leftRight;
		Halfedge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
		Edge* edge;

		Rectangle& dataBounds = _sites.getSitesBounds();

		int sqrt_nsites = (int)sqrt(_sites.length() + 4);
		HalfedgePriorityQueue heap(dataBounds.y, dataBounds.height, sqrt_nsites);
		EdgeList edgeList(dataBounds.x, dataBounds.width, sqrt_nsites);
		std::vector< Halfedge* > halfEdges;
		std::vector< Vertex* > vertices;

		Site* bottomMostSite = _sites.next();
		newSite = _sites.next();

		while( true )
		{
/*			if(heap.empty() == false)
			{
				newintstar = heap.min();
			}

			if (newSite != null
			&&  (heap.empty() || compareByYThenX(newSite, newintstar) < 0))
			{
				 new site is smallest
				//trace("smallest: new site " + newSite);

				// Step 8:
				lbnd = edgeList.edgeListLeftNeighbor(newSite.coord);	// the Halfedge just to the left of newSite
				//trace("lbnd: " + lbnd);
				rbnd = lbnd.edgeListRightNeighbor;		// the Halfedge just to the right
				//trace("rbnd: " + rbnd);
				bottomSite = rightRegion(lbnd);		// this is the same as leftRegion(rbnd)
				// this Site determines the region containing the new site
				//trace("new Site is in region of existing site: " + bottomSite);

				// Step 9:
				edge = Edge.createBisectingEdge(bottomSite, newSite);
				//trace("new edge: " + edge);
				_edges.push(edge);

				bisector = Halfedge.create(edge, LR.LEFT);
				halfEdges.push(bisector);
				// inserting two Halfedges into edgeList constitutes Step 10:
				// insert bisector to the right of lbnd:
				edgeList.insert(lbnd, bisector);

				// first half of Step 11:
				if ((vertex = Vertex.intersect(lbnd, bisector)) != null)
				{
					vertices.push(vertex);
					heap.remove(lbnd);
					lbnd.vertex = vertex;
					lbnd.ystar = vertex.y + newSite.dist(vertex);
					heap.insert(lbnd);
				}

				lbnd = bisector;
				bisector = Halfedge.create(edge, LR.RIGHT);
				halfEdges.push(bisector);
				// second Halfedge for Step 10:
				// insert bisector to the right of lbnd:
				edgeList.insert(lbnd, bisector);

				// second half of Step 11:
				if ((vertex = Vertex.intersect(bisector, rbnd)) != null)
				{
					vertices.push(vertex);
					bisector.vertex = vertex;
					bisector.ystar = vertex.y + newSite.dist(vertex);
					heap.insert(bisector);
				}

				newSite = _sites.next();
			}
			else if (heap.empty() == false)
			{
				 intersection is smallest
				lbnd = heap.extractMin();
				llbnd = lbnd.edgeListLeftNeighbor;
				rbnd = lbnd.edgeListRightNeighbor;
				rrbnd = rbnd.edgeListRightNeighbor;
				bottomSite = leftRegion(lbnd);
				topSite = rightRegion(rbnd);
				// these three sites define a Delaunay triangle
				// (not actually using these for anything...)
				//_triangles.push(new Triangle(bottomSite, topSite, rightRegion(lbnd)));

				v = lbnd.vertex;
				v.setIndex();
				lbnd.edge.setVertex(lbnd.leftRight, v);
				rbnd.edge.setVertex(rbnd.leftRight, v);
				edgeList.remove(lbnd);
				heap.remove(rbnd);
				edgeList.remove(rbnd);
				leftRight = LR.LEFT;
				if (bottomSite.y > topSite.y)
				{
					tempSite = bottomSite; bottomSite = topSite; topSite = tempSite; leftRight = LR.RIGHT;
				}
				edge = Edge.createBisectingEdge(bottomSite, topSite);
				_edges.push(edge);
				bisector = Halfedge.create(edge, leftRight);
				halfEdges.push(bisector);
				edgeList.insert(llbnd, bisector);
				edge.setVertex(LR.other(leftRight), v);
				if ((vertex = Vertex.intersect(llbnd, bisector)) != null)
				{
					vertices.push(vertex);
					heap.remove(llbnd);
					llbnd.vertex = vertex;
					llbnd.ystar = vertex.y + bottomSite.dist(vertex);
					heap.insert(llbnd);
				}
				if ((vertex = Vertex.intersect(bisector, rrbnd)) != null)
				{
					vertices.push(vertex);
					bisector.vertex = vertex;
					bisector.ystar = vertex.y + bottomSite.dist(vertex);
					heap.insert(bisector);
				}
			}
			else
			{
				break;
			}
		}

		// heap should be empty now
		heap.dispose();
		edgeList.dispose();

		for each (var halfEdge:Halfedge in halfEdges)
		{
			halfEdge.reallyDispose();
		}
		halfEdges.length = 0;

		// we need the vertices to clip the edges
		for each (edge in _edges)
		{
			edge.clipVertices(_plotBounds);
		}
		// but we don't actually ever use them again!
		for each (vertex in vertices)
		{
			vertex.dispose();
		}
		vertices.length = 0;

		function leftRegion(he:Halfedge):Site
		{
			var edge:Edge = he.edge;
			if (edge == null)
			{
				return bottomMostSite;
			}
			return edge.site(he.leftRight);
		}

		function rightRegion(he:Halfedge):Site
		{
			var edge:Edge = he.edge;
			if (edge == null)
			{
				return bottomMostSite;
			}
			return edge.site(LR.other(he.leftRight));
		}*/
	}


} /* namespace Delaunay */
