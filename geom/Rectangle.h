/*
 * Rectangle.h
 *
 *  Created on: 06.03.2013
 *
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "defines.h"

namespace Delaunay
{

	class Rectangle
	{
	public:
		Rectangle( );
		~Rectangle( );

		inline Number bottom( ) const
		{
			return Y + Height;
		}

		inline Number left( ) const
		{
			return X;
		}

		inline Number right( ) const
		{
			return X + Width;
		}

		inline Number top( ) const
		{
			return Y;
		}

		inline Number x( ) const
		{
			return X;
		}

		inline Number y( ) const
		{
			return Y;
		}

		inline Number width( ) const
		{
			return Width;
		}

		inline Number height( ) const
		{
			return Height;
		}

	private:
		Number X, Y, Width, Height;
	};

} /* namespace Delaunay */
#endif /* RECTANGLE_H_ */
