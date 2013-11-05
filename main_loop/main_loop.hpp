
#ifndef MAIN_LOOP_HPP
#define MAIN_LOOP_HPP

#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/image.hpp"
#include "display/display.hpp"
#include "stat/stat.hpp"

class CMainLoop
{
	CLua & _lua;

	public:

		CSecPerFrameStat sec_per_frame; // Секунд на кадр

		CMainLoop(CLua & lua);
		~CMainLoop();

		void operator()(const QString src_fname, const QString dst_fname);

};

#endif

