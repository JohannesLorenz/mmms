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

#ifndef ZYNADDSUBFX_H
#define ZYNADDSUBFX_H

#include "instrument.h"

namespace mini
{


class zynaddsubfx_t;

namespace zyn {

template<class InstClass>
class node_t : public named_t
{
public: // TODO
	InstClass* ins;
// the inheriting class must define the sub-nodes
protected:
	template<class NodeT>
	NodeT spawn(const std::string& ext) const {
		return NodeT(ins, name(), ext);
	}

	template<class NodeT>
	NodeT spawn(const std::string& ext, std::size_t id) const {
		return spawn<NodeT>(ext + std::to_string(id));
	}
public:
	node_t(InstClass* ins, const std::string& base, const std::string& ext)
		: named_t(base + ext), ins(ins) {}



	//node(std::string base, std::string ext, std::size_t id)
	//	: node(base, ext + std::to_string(id)) {}
};

template<class T>
class node_port_t : node_t<T>
{
	// todo: disallow spawn here
	template<class NodeT>
	NodeT spawn(const std::string& ext, std::size_t id) const {
		return spawn<NodeT>(ext + std::to_string(id));
	}

	using node_t<T>::node_t;
};


/*class p_envsustain : node_port_t
{
public:
	using node_port_t::node_port_t;
};*/



/*template<class Port1 = no_port<int>>
class p_envsustain : public command<oint<Port1>>
{
	using base = command<oint<Port1>>;
public:
	static const char* path() { return "PEnvsustain"; } // TODO: noteOn string is code duplicate
	p_envsustain(oint<Port1> value) // TODO: "ref?"
		: base("/PEnvsustain", value)
	{
	}
};*/



template<class PortType, class InstClass>
struct in_port_with_command : node_t<InstClass>, PortType
{ // TODO: instrument.h
	// a bit non-conform to store the command here, but working...
	//using base = command<oint<Port1>>;

	command_base* cmd;
public:
	in_port_with_command(InstClass* ins, const std::string& base, const std::string& ext, command_base* cmd) :
		node_t<InstClass>(ins, base, ext),
		PortType(*ins),
		cmd(cmd)
	{
		static_cast<instrument_t*>(ins)->add_in_port(this);
	}

/*out_port_with_command(oint<Port1> value) // TODO: "ref?"
		: base("/PEnvsustain", value)
	{
	}*/
};

using znode_t = node_t<zynaddsubfx_t>;

template<class PortT>
struct p_envsustain : public in_port_with_command<in_port_templ<int>, zynaddsubfx_t>
{
	using base = in_port_with_command<in_port_templ<int>, zynaddsubfx_t>;
	p_envsustain(zynaddsubfx_t* ins, const std::string& base, const std::string& ext) :
		in_port_with_command<in_port_templ<int>, zynaddsubfx_t>(ins, base, ext, new command<oint<PortT>>(ext.c_str(), *this)) {
	}
};

template<class PortT>
struct p_note_input : public in_port_with_command<in_port_templ<note_signal_t>, zynaddsubfx_t>
{
/*	p_envsustain(zynaddsubfx_t* ins, const std::string& base, const std::string& ext) :
		in_port_with_command<in_port_templ<int>, zynaddsubfx_t>(ins, base, ext, new command<note_signal_t<PortT>>(ext.c_str(), *this)) {
	}*/

};


#if 0
ss znode_t : node_t
{
	/*zynaddsubfx_t& zyn;
	znode_t(const zynaddsubfx_t& zyn, const std::string& base, const std::string& ext)
		: node_t(base, ext), zyn(zyn) {}*/
}
#endif

class amp_env : znode_t
{
public:
	using znode_t::znode_t;
	/*template<class Port1>*/
/*	zyn::p_envsustain envsustain() const {
		ins->add_in_port(new );
		//return p_envsustain();
	}*/
	template<class Port>
	zyn::p_envsustain<Port> envsustain() const {
		return spawn<zyn::p_envsustain<Port>>("PEnvsustain");
	}

	/*template<class Port1>
	zyn::p_envsustain<Port1> envsustain(oint<Port1> con) const {
		return p_envsustain<Port1>(con);
	}*/
};

class global : znode_t
{
public:
	using znode_t::znode_t;
	zyn::amp_env amp_env() const {
		return spawn<zyn::amp_env>("AmpEnvelope/");
	}
};


class voice0 : znode_t
{
public:
	using znode_t::znode_t;
};

class adpars : znode_t
{
public:
	using znode_t::znode_t;
	zyn::voice0 voice0() const {
		return spawn<zyn::voice0>("voice0/");
	}
	zyn::global global() const {
		return spawn<zyn::global>("global/");
	}
};

}

struct zyn_impl : is_impl_of_t<zynaddsubfx_t>, protected work_queue_t
{
	using is_impl_of_t<zynaddsubfx_t>::is_impl_of_t;

	class port_work : task_base
	{
		in_port_base* ip;
	public:
		port_work(in_port_base* ip) : task_base(0.0f), ip(ip) {}
		void proceed(float time) {
			(void)time;
			(void)ip;
// TODO
		}
	};

	zyn_impl(zynaddsubfx_t* ref);

	float proceed(float time) {
		return work_queue_t::run_tasks(time);
	}
};

class zynaddsubfx_t : public zyn::znode_t, public instrument_t, has_impl_t<zyn_impl>
{
	// TODO: read from options file
/*	const char* binary
		= "/tmp/cprogs/fl_abs/gcc/src/zynaddsubfx";
	const char* default_args = "--no-gui -O alsa";*/

	using has_impl_t = has_impl_t<zyn_impl>;
public:

	/*template<template<class> C1, template<class> C2>
	class note_on : public command<int_f, int_f, int_f> { //using command::command;
	public:
		note_on(con<p_char> x, con<p_char> y, con<p_char> z) : command("/noteOn", x, y, z) {} // TODO: a bit much work?
	};*/

	float proceed(float time) { return impl->proceed(time); }


	std::string make_start_command() const;
	cmd_vectors make_note_commands(const std::multimap<daw::note_geom_t, daw::note_t>& mm) const;

	port_t get_port(pid_t pid, int ) const;
	zynaddsubfx_t(const char* name);
	virtual ~zynaddsubfx_t() {} //!< in case someone derives this class

	// TODO: string as template param?
/*	class note_on : public command<p_char, p_char, p_char>
	{
		static const char* path() { return "/noteOn"; }
		template<class ...Args>
		note_on(const char* _path, Args ...args) : command(_path, ...args) {}
	};*/

	//class note_off : public command<p_char, p_char> { static const char* path() { return "/noteOff"; } };
	template<class Port1 = no_port<int>, class Port2 = no_port<int>, class Port3 = no_port<int>>
	class note_on : public command<oint<Port1>, oint<Port2>, oint<Port3>>
	{
		using base = command<oint<Port1>, oint<Port2>, oint<Port3>>;
	public:
		static const char* path() { return "/noteOn"; } // TODO: noteOn string is code duplicate
		note_on(oint<Port1> chan, oint<Port2> note, oint<Port3> velocity)
			: base("/noteOn", chan, note, velocity)
		{
		}
	};

	zyn::adpars add0() const {
		return spawn<zyn::adpars>("part0/kit0/adpars/");
	}

//	in_port_templ<note_signal_t> note_input;
	template<class Port>
	zyn::p_envsustain<Port> note_input() const {
		return spawn<zyn::p_envsustain<Port>>("PEnvsustain");
	}
};

}

#endif // ZYNADDSUBFX_H
