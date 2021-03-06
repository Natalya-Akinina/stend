
#include "main_loop/display.hpp"
#include "ui/gui/ui/main_window.hpp"

unsigned delay;
CMainWindow * p_main_window;

// ############################################################################ 

void CDisplay::init()
{
	;
}

void CDisplay::destroy()
{
	;
}

void CDisplay::reset(const unsigned fps)
{
	delay = 1000 / fps;
}

QString CDisplay::get_fname_to_save_stat()
{
	return p_main_window->get_fname_to_save_stat();
}

void CDisplay::show_Mat(const Mat & src, const Mat & dst)
{
	p_main_window->display_Mat(src, dst);

	QTest::qWait(delay);
}

void CDisplay::show_png(const QString & name_ru, const QString & fname)
{
	p_main_window->display_png(name_ru, fname);
}

