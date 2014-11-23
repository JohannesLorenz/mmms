/*************************************************************************/
/* mmms - minimal multimedia studio                                      */
/* Copyright (C) 2014-2014                                               */
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

#include <cstring>
#include "rtosc_string.h"

namespace mmms {

void inspect_rtosc_string(const std::vector<char>& str,
	std::ostream& stream) {

	for(const char& x : str)
	{
		stream << +x << std::endl;
	}
	stream << std::endl;

	if(str[0] != '/')
	 throw "rtosc string invalid: does not start with `/'";
	stream << "rtosc msg: \"" << str.data() << "\"" << std::endl;
	std::vector<char>::const_iterator itr = str.begin() + strlen(str.data());
	for(; !*itr; ++itr) ;
	if(*(itr++)!=',')
	 throw "rtosc string invalid: type string does not start with `,'";
	const char* args = &*itr;
	itr += strlen(&*itr);
	for(; !*itr; ++itr) ;
	for(; *args && (itr != str.end()); ++args)
	{
		stream << " * ";
		const char* c = &*itr;
		switch(*args)
		{
			case 'i':
				stream << (int)(*(int32_t*)c);
				itr += 4;
				break;
			case 'f':
				stream << *(float*)c;
				itr += 4;
				break;
			default:
				stream << "(unknown type: " << *args << ")";
		}
		stream << std::endl;
		//std::cerr << "c - str.data(): " << c - str.data() << std::endl;
	}

	if(*args) // i.e. itr reached end too early
	{
		stream << " -> rtosc string not terminated here." << std::endl;
	}


	// TODO
}

}
