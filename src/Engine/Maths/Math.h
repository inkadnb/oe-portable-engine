/*****************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
*****************************************************************************************/
#ifndef MATH_H_
#define MATH_H_

#define PI 3.14159265358
#define PI2 1.57079632679
#define PIX2 6.28318530716

namespace OE
{
	namespace Maths
	{
		class Math
		{
		public:
			static inline float SafeInverse(float x)
			{
				return x != 0 ? 1/x : 0;
			}
		};
	}
}

#endif /*MATH_H_*/
