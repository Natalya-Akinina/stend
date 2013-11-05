
#include "display/display.hpp"

CDisplay::CDisplay(const unsigned fps) :
	__fps(fps)
{
	;
}

CDisplay::~CDisplay()
{
	;
}

// ############################################################################ 

COpenCVDisplay::COpenCVDisplay(const unsigned fps) :
	CDisplay(fps)
{
	namedWindow(src_wname, CV_WINDOW_NORMAL);
	namedWindow(dst_wname, CV_WINDOW_NORMAL);

	delay = 1000 / fps;
}

COpenCVDisplay::~COpenCVDisplay()
{
	waitKey(-1);
}

void COpenCVDisplay::operator()(const Mat & src, const Mat & dst)
{
	__src = src.clone();
	__dst = dst.clone();

	imshow(src_wname, __src);
	imshow(dst_wname, __dst);

	waitKey(delay);
}

bool COpenCVDisplay::is_run()
{
	return true;
}

void COpenCVDisplay::stop()
{
	;
}

// ############################################################################ 

CQt5Display::CQt5Display(QMdiSubWindow * src_window, QMdiSubWindow * dst_window, bool * p_is_run, const unsigned fps) :
	CDisplay(fps)
{
	__src_window = src_window;
	__dst_window = dst_window;
	__p_is_run = p_is_run;
	delay = 1000 / fps;
}

CQt5Display::~CQt5Display()
{
	;
}

void CQt5Display::operator()(const Mat & src, const Mat & dst)
{
	CQt5Display::display(__src_window, src);
	CQt5Display::display(__dst_window, dst);

	QTest::qWait(delay);
}

bool CQt5Display::is_run()
{
	return * __p_is_run;
}

void CQt5Display::stop()
{
	(* __p_is_run) = false;
}

void CQt5Display::display(QMdiSubWindow * window, const Mat & img)
{
	Mat rgb;
	const QSize size(img.cols, img.rows);

	cvtColor(img, rgb, CV_BGR2RGB);

	((QLabel *) window->widget())->setPixmap
	(
		QPixmap::fromImage
		(
			QImage((const unsigned char *) rgb.data, rgb.cols, rgb.rows, QImage::Format_RGB888)
		)
	);

	window->resize(size);
	window->setMaximumSize(size);
}

