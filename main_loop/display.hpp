
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "all.hpp"

class CDisplay
{
	public:

		static void init();
		static void destroy();
		static void reset(const unsigned fps);
		static void show(const Mat & src, const Mat & dst);
};

#endif

