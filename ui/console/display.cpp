
#include "main_loop/display.hpp"

unsigned delay;
const string src_wname = "Source", dst_wname = "Result";
Mat __src, __dst;

void CDisplay::init()
{
	namedWindow(src_wname, CV_WINDOW_NORMAL);
	namedWindow(dst_wname, CV_WINDOW_NORMAL);
}

void CDisplay::destroy()
{
	waitKey(-1);
	destroyAllWindows();
}

void CDisplay::reset(const unsigned fps)
{
	delay = 1000 / fps;
}

void CDisplay::show(const Mat & src, const Mat & dst)
{
	__src = src.clone();
	__dst = dst.clone();

	imshow(src_wname, __src);
	imshow(dst_wname, __dst);

	waitKey(delay);
}

