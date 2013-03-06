/*
 * LR.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef LR_H_
#define LR_H_

namespace Delaunay
{
	namespace LR
	{
		enum Side {
			LEFT = 0, RIGHT
		};

		int other( enum Side leftRight )
		{
			return leftRight == LEFT ? RIGHT : LEFT;
		}
	};

} /* namespace Delaunay */
#endif /* LR_H_ */
