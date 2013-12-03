
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "all.hpp"

class CDisplay
{
	public:

		static void init();
		static void destroy();
		static void reset(const unsigned fps);
		static QString get_fname_to_save_stat();
		static void show_Mat(const Mat & src, const Mat & dst);
		static void show_png(const QString & name_ru, const QString & fname);
};

#endif

