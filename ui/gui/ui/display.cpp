
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

void CDisplay::show(const Mat & src, const Mat & dst)
{
	p_main_window->display(src, dst);

	QTest::qWait(delay);
}

