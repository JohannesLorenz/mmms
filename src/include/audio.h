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

#ifndef AUDIO_H
#define AUDIO_H

#include "ringbuffer/src/lib/ringbuffer.h"
#include "ports.h"

namespace mini
{

template<class T, std::size_t N = 2>
using multiplex = std::array<T, N>;

struct audio_out : out_port_templ<multiplex<ringbuffer_t>>
{
	using base::out_port_templ;
};

struct audio_in : in_port_templ<multiplex<ringbuffer_t>>
{
	using base::in_port_templ;
};

}

#endif // AUDIO_H