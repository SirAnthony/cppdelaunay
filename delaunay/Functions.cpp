/*
 * Functions.cpp
 *
 *  Created on: 06.03.2013
 */

#include "Functions.h"

namespace Delaunay {

	std::vector<LineSegment*> delaunayLinesForEdges( std::vector<Edge*>& edges )
	{
		std::vector<LineSegment*> segments;
		for( std::vector<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it ){
			segments.push_back( ( *it )->delaunayLine() );
		}
		return segments;
	}

	std::vector<LineSegment*> visibleLineSegments( std::vector<Edge*>& edges )
	{
		std::vector<LineSegment*> segments;
		for( std::vector<Edge*>::iterator it = edges.begin(); it < edges.end(); ++it ){
			Edge* edge = ( *it );

			if( edge->visible() ){
				Point* p1 = edge->clippedEnds()[LR::LEFT];
				Point* p2 = edge->clippedEnds()[LR::RIGHT];
				segments.push_back( LineSegment::create( p1, p2 ) );
			}
		}

		return segments;
	}

	std::vector<Edge*> selectEdgesForSitePoint( const Point* coord,
			std::vector<Edge*>& edges )
	{
		std::vector<Edge*> n;
		for( std::vector<Edge*>::iterator it = edges.begin(); it < edges.end(); ++it ){
			Edge* edge = ( *it );
			if( ( edge->leftSite() && edge->leftSite()->coord() == coord )
					|| ( edge->rightSite() && edge->rightSite()->coord() == coord ) )
				n.push_back( edge );
		}
		return n;
	}

	std::vector<Edge*> selectNonIntersectingEdges( //keepOutMask:BitmapData,
			std::vector<Edge*>& edgesToTest )
	{
		//if (keepOutMask == null)
		return edgesToTest;
		/*
		var zeroPoint:Point = new Point();
		return edgesToTest.filter(myTest);

		function myTest(edge:Edge, index:int, vector:Vector.<Edge>):Boolean
		{
		var delaunayLineBmp:BitmapData = edge.makeDelaunayLineBmp();
		var notIntersecting:Boolean = !(keepOutMask.hitTest(zeroPoint, 1, delaunayLineBmp, zeroPoint, 1));
		delaunayLineBmp.dispose();
		return notIntersecting;
		}*/
	}

} /* namespace Delaunay */
