/*************************************************************************/
/* test files for minimal                                                */
/* Copyright (C) 2014-2015                                               */
/* Johannes Lorenz (jlsf2013 @ sourceforge)                              */
/*                                                                       */
/* This program is free software; you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation; either version 3 of the License, or (at */
/* your option) any later version.                                       */
/* This program is distributed in the hope that it will be useful, but   */
/* WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      */
/* General Public License for more details.                              */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program; if not, write to the Free Software           */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA  */
/*************************************************************************/

#include <limits>
#include <cstdint>
#include "bars.h"
#include "io.h"

using namespace mini;
using namespace mini::bars;

template<class T1, class T2>
void throw_if_neq(const T1& x, const T2& y)
{
	if(!(x == y))
	{
		no_rt::mlog << "Error - not equal: " << x << " != " << y
			<< std::endl;
		throw "Elements not equal as expected";
	}
}

using namespace bars;

int main()
{
	try {
		throw_if_neq(lcm(42, 105), 210);
		throw_if_neq(gcd(42, 105), 21);
		
		throw_if_neq(bars_t(42,105), bars_t(2,5));
		throw_if_neq(bars_t(1,2) + bars_t(1, 3), bars_t(5,6));
		
		throw_if_neq(bars_t(42,105).floor(), 0);
		throw_if_neq(bars_t(21,4).floor(), 5);
		throw_if_neq(bars_t(23,4).rest(), bars_t(3,4));

		throw_if_neq(as_samples_floor(bars_t(1, 128), 1024), 8);

		throw_if_neq(1 + 1_3, bars_t(4, 3));

		throw_if_neq(0_2, 0_3);

		throw_if_neq(bars_t(std::numeric_limits<int_least64_t>::max(),1)
			> bars_t(1,1), true);

		constexpr bars_t b(2, 4);
		(void)b;

	} catch (const char* s)
	{
		no_rt::mlog << s << std::endl;
		return 1;
	}

	no_rt::mlog << "SUCCESS" << std::endl;
	return 0;
}


