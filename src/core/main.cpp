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
#include <cassert>
#include <unistd.h>
#include <termios.h>

#include "plugin.h"
#include "io.h"
#include "project.h"
#include "jack_engine.h"

using namespace mini;

void main_init()
{
	// TODO: needed?
	//For misc utf8 chars
	setlocale(LC_ALL, "");

	struct termios ctrl;
	tcgetattr(STDIN_FILENO, &ctrl);
	// turn off canonical mode => input unbuffered
	ctrl.c_lflag &= ~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);
}

/*project_t main_load_project(const char* lib_name)
{
	plugin_t plugin = plugin_t(lib_name);

	project_t pro;

	std::cout << "Attempting to load project: " << lib_name << std::endl;
	plugin.load_project(pro); // TODO: return pro?

	std::cout << "Loaded project: " << pro.title() << std::endl;

//	loaded_project lo_pro(project_t(pro));

	return pro;
}*/

using namespace bars;

void dump_error(const char* error_str)
{
	no_rt::mlog << "Aborting on error thrown: " << std::endl
		<< error_str << std::endl;
}

int main(int argc, char** argv)
{
	int errors = 0;
	try {
		main_init();

		assert(argc == 2);

		//std::cout << "main:" << main_load_project(argv[1]).title() << std::endl;

		plugin_t plugin(argv[1]);
		project_t pro;

		plugin.load_project(pro);
		no_rt::mlog << pro.effects().size() << std::endl;
		pro.finalize(); // TODO: in plugin.load... ?

		jack_engine_t eng;
		eng.load_project(pro);
		//eng.play_until(5_1);
		eng.activate();
		eng.run();

		while(eng.is_running()) {
			usleep(500000); // 0.5 s (hopefully!)
		}

		no_rt::mlog << "Main thread exiting normally..." << std::endl;

	} catch(const char* msg) {
		++errors; dump_error(msg);
	} catch(std::string msg) {
		++errors; dump_error(msg.c_str());
	} catch(std::exception e) {
		++errors;
		no_rt::mlog << "Caught std::exception: " << e.what()
			<< std::endl;
	} catch(...) {
		++errors; dump_error("unimplemented exception type");
	}

	return errors
		? EXIT_FAILURE
		: EXIT_SUCCESS;
}
