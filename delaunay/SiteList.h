/*
 * SiteList.h
 *
 *  Created on: 06.03.2013
 */

#ifndef DELAUNAY_SITELIST_H_
#define DELAUNAY_SITELIST_H_

#include "geom/Circle.h"
#include "delaunay/Site.h"
#include "defines.h"

namespace Delaunay {

	class SiteList
	{
	public:
		SiteList( );
		~SiteList( );

		unsigned push( Site* site );
		unsigned length()
		{
			return _sites.size();
		}

		Site* next();
		Rectangle sitesBounds();
		std::vector< unsigned > siteColors( /*referenceImage:BitmapData = null*/ );
		std::vector< const Point* > siteCoords();

		/**
		*
		* @return the largest circle centered at each site that fits in its region;
		* if the region is infinite, return a circle of radius 0.
		*
		*/
		std::vector< Circle* > circles();

		std::vector< std::vector< Point*> > regions( const Rectangle& );
		void regionsPrepare( const Rectangle& );


		/**
		 *
		 * @param proximityMap a BitmapData whose regions are filled with the site index values; see PlanePointsCanvas::fillRegions()
		 * @param x
		 * @param y
		 * @return coordinates of nearest Site to (x, y)
		 *
		 */
		const Point* nearestSitePoint( /*proximityMap:BitmapData,*/ Number, Number );

	private:
		std::vector< Site* > _sites;
		unsigned _currentIndex;
		bool _sorted;


	};

} /* namespace Delaunay */
#endif /* DELAUNAY_SITELIST_H_ */
