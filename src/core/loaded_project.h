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
#include "sample.h"
#include "bars.h"

//#include "jack_engine.h"

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


class loaded_project_t;

//! rt-safe stack, once reserved
template<class T>
class array_stack
{
	std::vector<T> data;
public:
	void reserve(const std::size_t& size) { data.reserve(size); }
	void push(const T& new_value) { data.push_back(new_value); }
	T&& pop() {
		T&& top = std::move(*data.rbegin());
		data.pop_back();
		return std::move(top);
	}
	std::size_t size() const { return data.size(); }
};

class _player_t : public work_queue_t // TODO: own header
{
	// TODO: class handle_work_queue_t?
	std::map<const effect_t*, handle_type> handles;

	array_stack<effect_t*> ready_fx;

/*	//!< maximum seconds to sleep until wakeup forced
	//!< @deprecated deprecated?
	static constexpr const sample_t max_sleep_time = 0.1;

	sample_t step = 0.001f; //0.001seconds;
	sample_t pos = 0.0f;*/
	sample_t pos = 0; //!< number of samples played until now
	loaded_project_t& project; // TODO! must be const

//	std::set<sample_t> end_set = { std::numeric_limits<sample_t>::max() };

/*	class task_events : public task_base
	{
	//	const loaded_project_t& project;
		const loaded_instrument_t* ins;
		const command_base* cmd;
		std::set<sample_t>::const_iterator itr;
	public:
		void proceed(sample_t); // TODO: really cpp?

		//sample_t next() { return *itr; }
		// TODO: no idea why I can not use initializer lists
		task_events(//const loaded_project_t& project,
			const loaded_instrument_t* ins,
			const command_base* cmd,
			const std::set<sample_t>::const_iterator& itr) :
			task_base(*itr),
			//project(project),
			ins(ins),
			cmd(cmd),
			itr(itr)
		{
		}
	};*/

	class task_effect : public task_base
	{
	//	const instrument_t* ins;
	public:
		effect_t* effect;
		std::vector<task_effect*> in_efcs, out_efcs;
		//const command_base* cmd;
	public:
		task_effect(effect_t* effect) :
			// TODO: 0 is wrong if we don't start playback at 0
			task_base(effect->get_next_time()),
			effect(effect)
		{
		}

		void proceed(sample_t time)
		{
			if(effect->proceed(time))
			{
				update_next_time(effect->get_next_time());
			}
			//else
			
			// the effect might have been finished or not
			// depending on which thread exits

			

			//update_next_time(effect->proceed(time));
		}

		bool cmp(const task_base& other) const {
			// ugly cast, but probably not avoidable?
			//return effect->id() < dynamic_cast<const task_effect&>(other).effect->id();
			const effect_t* const o_effect = dynamic_cast<const task_effect&>(other).effect;

			return bars_t(effect->cur_threads, effect->max_threads)
				< bars_t(o_effect->cur_threads, o_effect->max_threads);
			
		}

		handle_type& get_handle() final {

		}
	};

	void update_effects();
	void fill_commands();
	void send_commands();

	std::vector<std::vector<bool>> changed_ports;

	//engine_t* engine;

	void process(sample_t work);

	void init();
public:
	_player_t() = default;
	_player_t& operator=(loaded_project_t& _project);
	_player_t(loaded_project_t& _project);
	void play_until(sample_t dest);

	//! will be asking for the next nframes frames (i.e. samples for us)
	//! and announce the following callback at @a pos + @a nframes samples
	void callback(std::size_t nframes) {
		if(next_task_time() <= pos + (sample_t)nframes)
		 process(nframes);
	}
};

//! template type of @a _player_t
template<class SinkType>
class player_t : public _player_t
{
	const SinkType* sink;
public:
	using _player_t::_player_t;
};

// TODO: class audio_stereo_project will set sink to mult<ringbuffer_t>

class effect_root_t : public effect_t
{
	void instantiate() {}
	void clean_up() {}
	bool _proceed(sample_t ) { return true; }
};

//! this class takes a project and then does some things to handle it
class loaded_project_t : util::non_copyable_t
{
	friend class _player_t;

	// project
	project_t project;

	// connections
//	const std::vector<rtosc_con> _cons;
//	std::vector<rtosc_con> make_cons() const;
#if 0
	/*const*/ std::vector<loaded_instrument_t> _ins;
	std::vector<loaded_instrument_t> make_ins() const;
#endif
	effect_root_t _effect_root;

//	static mini::rtosc_con make_rtosc_con(const instrument_t &instrument);

	// commands
//	command_table commands;
	void init();

public:
//	const std::vector<loaded_instrument_t>& ins() const { return _ins; }
	effect_root_t& effect_root() { return _effect_root; }
	loaded_project_t() = default;
	loaded_project_t& operator=(project_t&& _project) noexcept;
	loaded_project_t(project_t&& _project) noexcept;
	~loaded_project_t();
};

}

#endif // LOADED_PROJECT_H
