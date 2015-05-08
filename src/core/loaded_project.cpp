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

#include <cstdlib>

#include "io.h"
#include "loaded_project.h"

namespace mini
{


/*rtosc_con loaded_project::make_rtosc_con(
	const instrument_t& instrument)
{
// TODO: move to rtosc_con ctor?
	rtosc_con con;
	get_input(instrument.make_start_command().c_str(), &con.pid);
	con.fd = 0; // TODO
	con.port = instrument.get_port(con.pid, con.fd);
	return con;
}*/


#if 0
std::vector<loaded_instrument_t> loaded_project_t::make_ins() const
{
	std::vector<loaded_instrument_t> result;
/*	for(const std::unique_ptr<instrument_t>& ins : project.instruments())
	{
		result.emplace_back(*ins);
	}*/
	return result;
}


void loaded_project_t::init()
{
	no_rt::mlog << "Loading project: " << this->project.title() << std::endl;
	std::cerr << "Found " << project.effects().size() << " effects." << std::endl;

	// instantiate and connect all fx
	for(effect_t* e : project.effects()) // TODO: -> initializer list
	{
		e->instantiate();
		if(e->writers.empty())
		{
			_effect_root.readers.push_back(e);
			e->writers.push_back(&_effect_root);
		}
	}

	// identify all fx
	//std::size_t next_id = 0;

	array_stack<effect_t*> ready_fx;
	ready_fx.push(&effect_root());
	do
	{
		effect_t* cur_effect = ready_fx.pop();

		if(cur_effect->id() != has_id::no_id())
		 throw "Id given twice";
		//cur_effect->set_id(next_id++);

		//std::cerr << "set id: " << next_id - 1 << " for " << cur_effect << std::endl;

		const auto cb = [&](const std::vector<effect_t*>& next_vector)
		{
			for(effect_t* next: next_vector)
			{
				bool parents_done = true;
				for(const effect_t* par: next->writers)
				 parents_done = parents_done && (par->id() != has_id::no_id());
				if(parents_done)
				 ready_fx.push(next);
			}
		};

		cb(cur_effect->readers);
		cb(cur_effect->deps);

	} while(ready_fx.size());

}

loaded_project_t& loaded_project_t::operator=(project_t&& ) noexcept // not sure if this is really noexcept...
{
	project = std::move(_project);

	init();
	return *this;
}

loaded_project_t::loaded_project_t(project_t&& _project) noexcept :
	project(std::move(_project))
	//_ins(std::move(make_ins()))
//	_global(daw_visit::visit(project.global()))
{
	init();
}

loaded_project_t::~loaded_project_t()
{
	for(effect_t* e : project.effects())
	 e->clean_up();

	for(const effect_t* e : project.effects())
	if(e != &effect_root()) // TODO: is this necessary?
	 delete e;

#if 0
	for(std::size_t i = 0; i < _ins.size(); ++i)
	{
		const auto& quit_commands = project.instruments()[i]->quit_commands();
		for(std::size_t j = 0; j < quit_commands.size(); ++j)
		{
			std::cout << "???" << std::endl;
		//	dump d;
		//	quit_commands[j]->execute(d);


		//	std::cout << quit_commands[j]->casted().arg(0) << std::endl;
		//	quit_commands[j]->call(cons[i].send_rtosc_msg); // TODO: without c_str()?
		}
	}
#endif
}
#endif

#if 0
void _player_t::update_effects()
{
	// TODO: use player's copies

	ready_fx.push(&project.effect_root());
	do
	{
		effect_t* cur_effect = ready_fx.pop();

		cur_effect->proceed(pos);

		for(effect_t* next: cur_effect->deps)
		 ready_fx.push(next);

	} while(ready_fx.size());
}
#endif

void _player_t::fill_commands()
{
/*	for(const auto& pr : global())
	for(const auto& pr2 : pr.second)
	{
		pr2.first->complete_buffer();
	}*/
}

void _player_t::send_commands()
{

}

void _player_t::init()
{
/*	for(const auto& pr : global())
	for(const auto& pr2 : pr.second)
	if(pr2.second.empty()) // marks a poll
	{
		//pq.push(new task_poll());
	}
	else
	{
		pq.push(new task_events(&pr.first, pr2.first, pr2.second.begin()));
	//	std::cerr << "pushing: " <<  *pr2.second.begin() << std::endl;
	}
	pq.push(new task_events(nullptr, nullptr, end_set.begin())); // = sentinel*/
	no_rt::mlog << "Player for " << project.title() << std::endl;

	project.emplace<sentinel_effect>(1 + project.effects().size());
	std::map<const effect_t*, handle_type> handles;

	no_rt::mlog << "FOUND " << project.effects().size() << " FX..." << std::endl;
	for(effect_t*& e : project.get_effects_noconst())
	{
		no_rt::mlog << "pushing effect " << e->name() << " (" << e->id() << "), next time: " << e->get_next_time() << std::endl;
		task_effect* new_task = new task_effect(e);
		handles[e] = add_task(new_task);
		std::cerr << e << std::endl;
		new_task->set_handle(handles[e]);
	}
	std::cerr << handles.size();
	for(auto& pr : handles)
	{
		task_effect* te = static_cast<task_effect*>(*pr.second);
		effect_t* e = te->effect;
		for(const out_port_base* op  : e->get_out_ports())
		for(in_port_base* target_ip : op->readers)
		{
			effect_t* target_ef = target_ip->e;
			//changed_ports[target_ef->id()][target_ip->id] = true;

			//handle_type h = handles.at(target_ef);
			handle_type h = handles[target_ef];
			/*(*h)->update_next_time(cur_next_time);
			update(h);*/
			te->out_efcs.push_back(dynamic_cast<task_effect*>(*h)); // TODO: cast correct/needed?


		}

		// TODO: use default atomic
		e->cur_threads.store(/*e->max_threads*/0); // marks this task as "done"
		no_rt::mlog << "Effect " << e->name() << ": threads: " << e->cur_threads << e->max_threads << std::endl;
	}

//	ready_fx.reserve(project.effects().size());

	changed_ports.resize(1 + project.effects().size());

/*	for(std::size_t i = 0; i < project.effects().size(); ++i)
	{
		//std::cerr << project.effects()[i]->name() << " -> " << project.effects()[i]->get_in_ports().size()
		//	<< std::endl;
		changed_ports

		changed_ports[i].resize(project.effects()[i]->get_in_ports().size());
	}*/

	for(effect_t* e : project.effects())
	{
		changed_ports[e->id()].resize(e->get_in_ports().size());
	}
}

_player_t::_player_t(loaded_project_t &_project) :
	project(_project)//,
//	engine(new jack_engine_t) // TODO: choice for engine
{
	init();
}

#ifdef __clang__
	#define REALTIME __attribute__((annotate("realtime")))
#else
	#define REALTIME // replace with "nothing"
#endif

// TODO: deprecated!?!?!?
void _player_t::play_until(sample_t dest)
{
// function probably deprecated

	sample_t final_pos = dest; //pos + work;
	std::cerr << "starting playback: " << pos << std::endl;
	for(; next_task_time() < final_pos; pos = next_task_time()) // TODO: <= ?
	{
//		usleep(1000000 * step);
		process(pos); // TODO: not 0
		std::cerr << "done: " << pos << std::endl;
		std::cerr << "next: " << next_task_time() << std::endl;
		std::cerr << "next name: " << ((task_effect*)peek_next_task())->effect->name() << std::endl;



		useconds_t sleep_time = std::min(next_task_time() - pos,
				dest - pos) * usecs_per_sample;
		io::mlog << "Sleeping for " << sleep_time << " useconds..." << io::endl;
		usleep(sleep_time);
	}

	// no more tasks possible, so:
	pos = final_pos;
}

//! the "heart" of minimal
void REALTIME _player_t::process(sample_t work)
{
//		update_effects();
//		fill_commands();
	//	send_commands();
	//
		(void)work; // TODO
		
//		const auto& has_active_tasks = []() -> bool {
//			
//		}

		while(peek_next_task()->next_time() <= pos &&
			dynamic_cast<task_effect*>(peek_next_task())->effect->cur_threads
			< dynamic_cast<task_effect*>(peek_next_task())->effect->max_threads )
		{
			// TODO: simple reinsert??

			task_base* top = peek_next_task();
			task_effect* task_e = dynamic_cast<task_effect*>(top);
			effect_t* this_ef = task_e->effect;

			/*const bool reinsert = top->proceed(pos);
			if(reinsert)
			 pq.push(top);*/
			const sample_t cur_next_time = top->next_time();
			this_ef->pass_changed_ports(changed_ports[this_ef->id()]);
			++this_ef->cur_threads;
			
			
			
			io::mlog << "next effect threads now: " << this_ef->cur_threads << io::endl;
			
			top->proceed(pos); // will also update the next-time event

			//handles.at(this_ef) = add_task(top);
			update(task_e->get_handle());

			/*for(const effect_t* dep : reinterpret_cast<task_effect*>(top)->effect->deps)
			{
				handle_type h = handles.at(dep);
				(*h)->update_next_time(cur_next_time);
				update(h);
			}*/


			std::size_t count = 0;
			// TODO: effect should give us this array...
			for(const out_port_base* op  : this_ef->get_out_ports())
			{
			// TODO! only dependencys...
			if(op->change_stamp <= pos)
			{
				for(in_port_base* target_ip : op->readers)
				{
				//	TODO: check this!!!
					effect_t* target_ef = target_ip->e;

					changed_ports[target_ef->id()][target_ip->id] = true;
					// TODO: use a vector in task_effect, too? like in_efcs, out_efcs?

					//handle_type h = handles.at(target_ef);
					handle_type h = task_e->out_efcs[count++]->get_handle(); //target_ef
					task_base* te = *h;

				//	te->effect
					te->update_next_time(cur_next_time);
					io::mlog << "next time: " << cur_next_time << io::endl;
					update(h);
				}
			}
			else
			 count +=op->readers.size();
			}
#if 0
			pq_entry top = std::move(pq.top());
			pq.pop();

			//project.cons()[top.ins]

			project.cons().front().send_osc_str(top.cmd->buffer());


			top.itr->operator ++();

			pq.push(std::move(top));

#endif
		//	if(top.itr == top.vals.end())
		//	 throw "end";

			//pq.decrease(pq.top());

			/*if(top().itr == top().vals.end())
			 pq.insert
			pq.pop();*/
		} // while has active tasks at this time
}

}

