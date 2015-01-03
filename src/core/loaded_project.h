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

#ifndef LOADED_PROJECT_H
#define LOADED_PROJECT_H

#include <unistd.h>
//#include <lo/lo_types.h>

#include "instrument.h"
#include "project.h"
#include "lo_port.h"
#include "types.h"
#include "lfo.h"
#include "daw_visit.h"
#include "work_queue.h"

namespace mini {

/*class loaded_instrument
{
	rtosc_con make_rtosc_con() const;
public:
	const instrument_t& instrument;
	rtosc_con con;
	loaded_instrument(const instrument_t& instrument) :
		instrument(instrument),
		con(make_rtosc_con())
	{
	}
};*/

using tick_t = std::size_t;

class m_time_t
{
	constexpr static double time_per_tick = 1000000 / 1024.0; // in useconds
public:
	tick_t pos; // represents 1/1024 seconds // suggested by fundamental
	void tick() { usleep(time_per_tick); }
	void tick(tick_t n_ticks) { usleep(time_per_tick * n_ticks); }
};

class bars_t
{
	tick_t n, c;
public:
	constexpr bars_t(tick_t n, tick_t c) : n(n), c(c) {}
	const bars_t operator+(const bars_t& rhs) const {
		return bars_t(n * rhs.c + rhs.n * c, c * rhs.c);
	}
};

namespace bars
{

constexpr bars_t _1(1, 1),
	_2(1, 2),
	_3(1, 3),
	_4(1, 4),
	_6(1, 6),
	_8(1, 8),
	_12(1, 12),
	_16(1, 16),
	_24(1, 24),
	_32(1, 32),
	_48(1, 18),
	_64(1, 64);

}

class loaded_project_t;


class player_t : public work_queue_t // TODO: own header
{
	//!< maximum seconds to sleep until wakeup forced
	//!< @deprecated deprecated?
	static constexpr const float max_sleep_time = 0.1;

	float step = 0.1f; //0.001seconds;
	float pos = 0.0f;
	loaded_project_t& project; // TODO! must be const

	std::set<float> end_set = { std::numeric_limits<float>::max() };

	/*struct pq_entry
	{
		//float next;
	//	const instrument_t* ins;
		const command_base* cmd;
		//const std::set<float>& vals;

		//const activator_base* const activator;
		activator_base_itr* const itr;



		//float next_val;
		//std::set<float>::const_iterator itr;

	};*/

	class task_events : public task_base
	{
	//	const loaded_project_t& project;
		const loaded_instrument_t* ins;
		const command_base* cmd;
		std::set<float>::const_iterator itr;
	public:
		void proceed(float); // TODO: really cpp?

		//float next() { return *itr; }
		// TODO: no idea why I can not use initializer lists
		task_events(//const loaded_project_t& project,
			const loaded_instrument_t* ins,
			const command_base* cmd,
			const std::set<float>::const_iterator& itr) :
			task_base(*itr),
			//project(project),
			ins(ins),
			cmd(cmd),
			itr(itr)
		{
		}
	};

	class task_effect : public task_base
	{
	//	const instrument_t* ins;
		effect_t* effect;
		//const command_base* cmd;
		task_effect() :
			// TODO: 0 is wrong if we don't start playback at 0
			task_base(0.0f)
		{

		}

		void proceed(float time) {
			float next_time = effect->proceed(time);
			update_next_time(next_time);
		}
	};

	/*struct pq_entry
	{
		task_base* task;
		float next_time;
	};*/

	void update_effects();
	void fill_commands();
	void send_commands();

public:
	player_t(loaded_project_t& project);

	void play_until(float dest);
};

class command_table
{


};

class effect_root_t : public effect_t {
	float _proceed(float ) { return 0.0f; }
};

//! this class takes a project and then does some things to handle it
class loaded_project_t : non_copyable_t
{
	// project
	project_t project;

	// connections
//	const std::vector<rtosc_con> _cons;
//	std::vector<rtosc_con> make_cons() const;
	/*const*/ std::vector<loaded_instrument_t> _ins;
	std::vector<loaded_instrument_t> make_ins() const;

	effect_root_t _effect_root, _new_effect_root;

//	static mini::rtosc_con make_rtosc_con(const instrument_t &instrument);

	// commands
//	command_table commands;

	// player
	// player_t player;

//	daw_visit::global_map _global;
public:
	const std::vector<loaded_instrument_t>& ins() const { return _ins; }

//	daw_visit::global_map& global() { return _global; }
	effect_root_t& effect_root() { return _effect_root; }
	effect_root_t& new_effect_root() { return _new_effect_root; }
	loaded_project_t(project_t&& project);
	~loaded_project_t();
private:
	std::pair<note_geom_t, note_t> visit(note_geom_t offset, const note_t &n) const;
	std::multimap<note_geom_t, note_t> visit(note_geom_t offset, const notes_t &ns) const;
//	cmd_vectors visit(const track_t &t) const;
//	global_map visit(global_t &g) const;
};

}

#endif // LOADED_PROJECT_H
