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

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <tuple>
#include <string>
#include <vector>

namespace mmms
{

//! note: if binary gets too large, we might need to not use templates...

class command_base
{
public:
	const char* path;
};

template<class ...Args>
class command : public command_base
{
public:
	std::tuple<Args...> args;
};

class instrument_t
{
	static std::size_t next_id;
	std::size_t id;
	std::vector<std::string> todo;
public:
	using port_t = int;
	instrument_t() : id(next_id++) {}
	enum class type
	{
		zyn
	};
	template<class ...Args>
	void add_param_fixed(const char* param, ...) {

	}

	void set_param_fixed(const char* param, ...);
	virtual std::string make_start_command() const = 0;
	//! shall return the lo port (UDP) after the program was started
	virtual port_t get_port(pid_t pid, int fd) const = 0;
};

class zynaddsubfx_t : public instrument_t
{
	// TODO: read from options file
	const char* binary
		= "/tmp/cprogs/fl_abs/gcc/src/zynaddsubfx";
	const char* default_args = "--no-gui -O alsa";
public:
	std::string make_start_command() const;
	port_t get_port(pid_t pid, int ) const;

};




}

#endif // INSTRUMENT_H
