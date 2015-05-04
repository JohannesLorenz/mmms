/*************************************************************************/
/* minimal - a minimal osc sequencer                                     */
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

#ifndef BARS_H
#define BARS_H

#include "fraction.h"
#include "sample.h"

namespace mini {

using bars_t = fraction_t<sample_t, sample_t>;

namespace bars
{

using num_t = unsigned long long int;
inline bars_t operator"" _1(num_t n) { return bars_t(n, 1); }
inline bars_t operator"" _2(num_t n) { return bars_t(n, 2); }
inline bars_t operator"" _3(num_t n) { return bars_t(n, 3); }
inline bars_t operator"" _4(num_t n) { return bars_t(n, 4); }
inline bars_t operator"" _8(num_t n) { return bars_t(n, 8); }

}

inline sample_t as_samples_floor(const bars_t& b, const sample_t& samples_per_bar)
{
	return bars_t(b.numerator(), b.denominator(), samples_per_bar).floor();
}

}

#endif // BARS_H
