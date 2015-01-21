/*************************************************************************/
/* minimal - a minimal osc sequencer                                     */
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

#include <unistd.h> // TODO
#include <fstream>

#include "lo_port.h"
#include "config.h"
#include "zynaddsubfx.h"

namespace mini {

std::string zynaddsubfx_t::make_start_command() const
{
	const std::string cmd = ZYN_BINARY
		" --no-gui -O alsa"; // TODO: read from options file
	return cmd;
}

cmd_vectors zynaddsubfx_t::make_note_commands(const std::multimap<daw::note_geom_t, daw::note_t> &mm) const
{
	// channel, note, velocity

	// note offset <-> command
	std::map<int, command_base*> cmd_of;

	cmd_vectors res;
	for(const std::pair<daw::note_geom_t, daw::note_t>& pr : mm)
	{
		//	res.emplace_back(new command<int_f, int_f, int_v>("/noteOn", 0, pr.first.offs, pr.second.velocity()), pr.first.start); // TODO: valgrind!

		auto itr1 = cmd_of.find(pr.first.offs);
		if(itr1 == cmd_of.end())
		{
			// TODO: valgrind
			command_base* cmd = new command<osc_int, osc_int, osc_int>("/noteOn", 0, pr.first.offs, pr.second.velocity());
			cmd_of.emplace_hint(itr1, pr.first.offs, cmd);

			// TODO: note_off

			res.emplace(cmd, std::set<float>{pr.first.start});
			std::cerr << "New note command: " << cmd << std::endl;

			cmd = new command<osc_int, osc_int>("/noteOff", 0, pr.first.offs);
			res.emplace(cmd, std::set<float>{pr.first.start + pr.second.length()});

			std::cerr << "Map content now: " << std::endl;
			for(const auto& p : res)
			{
				std::cerr << p.first->buffer() << std::endl;
			}
		}
		else
		{
			res.find(itr1->second)->second.insert(pr.first.start);
			std::cerr << "Found note command." << std::endl;
		}



	}
	std::cerr << "Added " << res.size() << " note commands to track." << std::endl;
	return res;
}

instrument_t::port_t zynaddsubfx_t::get_port(pid_t pid, int) const
{
	port_t port;
	std::string tmp_filename = "/tmp/zynaddsubfx_"
			+ std::to_string(pid);
	std::cout << "Reading " << tmp_filename << std::endl;
	sleep(1); // wait for zyn to be started... (TODO)
	std::ifstream stream(tmp_filename);
	if(!stream.good()) {
			throw "Error: Communication to zynaddsubfx failed.";
		}
	stream >> port;
	return port;
}

zynaddsubfx_t::zynaddsubfx_t(const char *name) :
	zyn::znode_t(this, "/", ""),
	instrument_t(name, { new command<>("/quit") }), // TODO! close-ui?
	m_impl(this),/*,
	note_input(*this)*/
	notes_t_port(this, "/", "/noteOn")
{
	/*using prt_t = in_port_templ<int>;
	for(int i = 0; i < NOTES_MAX; ++i) {
		commands.push_back(new command<oint<>, oint<prt_t>>("/notOn", i, oint<prt_t>(new prt_t(*this))));
	}*/

}

zyn_impl::zyn_impl(zynaddsubfx_t *ref) : is_impl_of_t<zynaddsubfx_t>::is_impl_of_t(ref)
{
/*	for(const out_port_base* op : ref->out_ports)
	{

	}*/ // TODO
}

void zyn::send_single_command(lo_port_t &lp, const osc_string &str)
{
	lp.send_raw(str.raw(), str.size());
}



}

