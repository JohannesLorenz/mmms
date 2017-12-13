/*************************************************************************/
/* minimal - a minimal osc sequencer                                     */
/* Copyright (C) 2014-2016                                               */
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

#include "io.h"
#include "event_roll.h"

#define DEBUG_NOTE_LINE

namespace mini
{

void log_note_event(bool on, const bars_t& start, int event_id) {
	io::mlog << "piano_roll: played note " <<
	 (on ? "on" : "off")
		 << " (start: " << start << ", event id: " << event_id << ")" << io::endl;
}

void roll_impl_base::log_visit_events(roll_impl_base::m_geom_t cur_offset,
	roll_impl_base::m_geom_t next_offset) const
{
#ifdef DEBUG_NOTE_LINE
	for(std::size_t x = visit_depth; x; --x)
	 io::mlog << "  ";
	io::mlog << "recursing: " << cur_offset + next_offset << io::endl;
#else
	(void)cur_offset;
	(void)next_offset;
#endif
}

void roll_impl_base::log_visit_event(roll_impl_base::m_geom_t next_offset,
	int next_visit_id) const
{
#ifdef DEBUG_NOTE_LINE
	for(std::size_t x = visit_depth; x; --x)
	 io::mlog << "  ";
	io::mlog << "emplacing: " << next_visit_id << io::endl;
	for(std::size_t x = visit_depth; x; --x)
	 io::mlog << "  ";
	io::mlog << "next_offs: " << next_offset << io::endl;
	// TODO: this does not work for all note properties
#else
	(void)next_offset;
	(void)next_visit_id;
#endif
}

}