
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

QString CDisplay::get_fname_to_save_stat()
{
	char buf[4096];

	printf("Enter fname >> ");
	scanf("%s", buf);
	
	return buf;
}

void CDisplay::show_Mat(const Mat & src, const Mat & dst)
{
	__src = src.clone();
	__dst = dst.clone();

	imshow(src_wname, __src);
	imshow(dst_wname, __dst);

	waitKey(delay);
}

void CDisplay::show_png(const QString & name_ru, const QString & fname)
{
	Mat img = imread(fname.toLocal8Bit().constData());

	throw_null(img.data, "Не удалось открыть png-файл");
	imshow("Results", img);
	waitKey(-1);
}

