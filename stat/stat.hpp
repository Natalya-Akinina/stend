
#ifndef STAT_HPP
#define STAT_HPP

#include "all.hpp"
#include "config.hpp"
#include "lib/lua.hpp"
#include "lib/image.hpp"

class CStat
{
	CLua & _lua;
	vector<double> sec_per_frame;

	public:

		CStat(CLua & lua);

		void run(const QString src_fname, const QString dst_fname);

		void display_sec_per_frame();
		void save_sec_per_frame(const QString fname);
};

#endif

