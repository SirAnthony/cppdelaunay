/*
 * Polygon.h
 *
 *  Created on: 06.03.2013
 */

#ifndef POLYGON_H_
#define POLYGON_H_

namespace Delaunay {

	namespace Winding {
		enum Direction {
			NONE = 0, CLOCKWISE, COUNTERCLOCKWISE,
		};
	}

	class Polygon
	{
	public:
		Polygon( std::vector<Point*>& vertices );
		~Polygon( );
		Number area( );
		Winding::Direction winding( );
		Number signedDoubleArea( );

	private:
		std::vector<Point*> _vertices;
	};

} /* namespace Delaunay */
#endif /* POLYGON_H_ */
