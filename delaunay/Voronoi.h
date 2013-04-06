/*
 * Voronoi.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef DELAUNAY_VORONOI_H_
#define DELAUNAY_VORONOI_H_

#include "geom/Triangle.h"
#include "delaunay/SiteList.h"
#include "delaunay/kruskal.h"
#include "geom/LineSegment.h"
#include <vector>
#include <map>

namespace Delaunay
{

	class Voronoi
	{
	public:
		Voronoi( const std::vector< Point* >& points,
				const std::vector< unsigned >* colors, Rectangle& plotBounds );
		~Voronoi( );

		static void clean();

		const Rectangle& plotBounds( );
		const std::vector< Edge* >& edges( );

		/** Points must be disposed. */
		std::vector< Point* > region( Point* p );

		// TODO: bug: if you call this before you call region(), something goes wrong :(
		std::vector< const Point* > neighborSitesForSite( Point* );

		//std::vector<Circle*> circles() { return _sites.circles(); }
		std::vector< LineSegment* > voronoiBoundaryForSite( Point* coord );
		std::vector< LineSegment* > delaunayLinesForSite( Point* coord );
		std::vector< LineSegment* > voronoiDiagram( );
		std::vector< LineSegment* > delaunayTriangulation( /* keepOutMask:BitmapData = null */);
		std::vector< LineSegment* > hull( );
		std::vector< const Point* > hullPointsInOrder( );
		std::vector< LineSegment* > spanningTree(
				enum KruskalType type = ktMinimum /* , keepOutMask:BitmapData = null */);
		std::vector< std::vector< Point* > > regions( );
		void regionsPrepare( );
		std::vector< unsigned > siteColors( /* referenceImage:BitmapData = null */);

		/**
		 *
		 * @param proximityMap a BitmapData whose regions are filled with the site index values; see PlanePointsCanvas::fillRegions()
		 * @param x
		 * @param y
		 * @return coordinates of nearest Site to (x, y)
		 *
		 */
		const Point* nearestSitePoint( /* proximityMap:BitmapData,*/Number x, Number y );
		std::vector< const Point* > siteCoords( );

		template<class T>
		static Number compareByYThenX( const Site* s1, T s2 );

	private:
		SiteList* _sites;
		std::map< Point*, Site* > _sitesIndexedByLocation;
		std::vector< Triangle > _triangles;
		std::vector< Edge* > _edges;

		// TODO generalize this so it doesn't have to be a rectangle;
		// then we can make the fractal voronois-within-voronois
		Rectangle _plotBounds;

		void addSites( const std::vector< Point* >&, const std::vector< unsigned >* );
		void addSite( Point* p, unsigned color, int index );
		std::vector< Edge* > hullEdges( );

		void fortunesAlgorithm( );

		static int _instances;
	};

} /* namespace Delaunay */
#endif /* VORONOI_H_ */
