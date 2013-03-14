/*
 * SiteList.cpp
 *
 *  Created on: 06.03.2013
 */

#include "delaunay/SiteList.h"
#include "delaunay/Edge.h"

namespace Delaunay {

	SiteList::SiteList( )
	{
		_sites.clear();
		_currentIndex = 0;
		_sorted = false;
	}

	SiteList::~SiteList( )
	{
		for( std::vector<Site*>::iterator it = _sites.begin(); it != _sites.end(); ++it )
			( *it )->dispose();
	}

	unsigned SiteList::push( Site* site )
	{
		_sorted = false;
		_sites.push_back( site );
		return _sites.size();
	}

	Site* SiteList::next( )
	{
		if( _sorted && _currentIndex < _sites.size() )
			return _sites[_currentIndex++];
		return NULL;
	}

	Rectangle SiteList::sitesBounds( )
	{
		if( !_sorted ){
			Site::sortSites( _sites );
			_currentIndex = 0;
			_sorted = true;
		}

		Number xmin, xmax, ymin, ymax;
		if( _sites.size() == 0 )
			return Rectangle( 0, 0, 0, 0 );
		xmin = DELAUNAY_NUMBER_MAX;
		xmax = DELAUNAY_NUMBER_MIN;
		for( std::vector<Site*>::iterator it = _sites.begin(); it != _sites.end(); ++it ){
			Site* site = ( *it );
			if( site->x() < xmin )
				xmin = site->x();
			if( site->x() > xmax )
				xmax = site->x();
		}
		// here's where we assume that the sites have been sorted on y:
		ymin = _sites[0]->y();
		ymax = _sites[_sites.size() - 1]->y();

		return Rectangle( xmin, ymin, xmax - xmin, ymax - ymin );
	}

	std::vector<unsigned> SiteList::siteColors( /*referenceImage:BitmapData = null*/)
	{
		std::vector<unsigned> colors;
		for( std::vector<Site*>::iterator it = _sites.begin(); it != _sites.end(); ++it )
			colors.push_back( ( *it )->color() );
		//referenceImage ? referenceImage.getPixel(site.x, site.y) :
		return colors;
	}

	std::vector<const Point*> SiteList::siteCoords( )
	{
		std::vector<const Point*> coords;
		for( std::vector<Site*>::iterator it = _sites.begin(); it != _sites.end(); ++it )
			coords.push_back( ( *it )->coord() );
		return coords;
	}

	std::vector<Circle*> SiteList::circles( )
	{
		std::vector<Circle*> circles;
		for( std::vector<Site*>::iterator it = _sites.begin(); it != _sites.end(); ++it ){
			Site* site = ( *it );
			Number radius = 0;
			Edge* nearestEdge = site->nearestEdge();
			if( !(nearestEdge->isPartOfConvexHull()) )
				radius = nearestEdge->sitesDistance() * 0.5;
			circles.push_back( new Circle( site->x(), site->y(), radius ) );
		}
		return circles;
	}

	std::vector< std::vector< Point* > > SiteList::regions( const Rectangle& plotBounds )
	{
		std::vector< std::vector< Point* > > regions;
		for( std::vector<Site*>::iterator it = _sites.begin(); it != _sites.end(); ++it )
			regions.push_back( (*it)->region(plotBounds) );
		return regions;
	}

	void SiteList::regionsPrepare( const Rectangle& plotBounds )
	{
		for( std::vector< Site* >::iterator it = _sites.begin(); it != _sites.end(); ++it )
			(*it)->regionPrepare( plotBounds );
	}

	const Point* SiteList::nearestSitePoint( /*proximityMap:BitmapData,*/ Number x, Number y )
	{
		//unsigned index = proximityMap.getPixel(x, y);
		//if(index > _sites.size() - 1)
			return NULL;
		//return _sites[index].coord();
	}

} /* namespace Delaunay */
