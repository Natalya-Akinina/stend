
#include "main_loop/main_loop.hpp"

CMainLoop::CMainLoop(CLua & lua) :
	_lua(lua)
{
	CStat * obj;

	throw_null(obj = new CSecPerFrameStat(measure), "Не удалось инициализировать замер времени обработки каждого кадра");
	stats[obj->name_en()] = PStat(obj);
}

CMainLoop::~CMainLoop()
{
	;
}

void CMainLoop::start(const QString src_fname, const QString dst_fname)
{
	try
	{
		unsigned counter;
		struct timespec ts_before, ts_after;
		Mat src_frame, * dst_frame;

		start();

		VideoCapture src(src_fname.toLocal8Bit().constData());
		throw_if(! src.isOpened(), "TODO");

		const double fps = src.get(CV_CAP_PROP_FPS);
		VideoWriter dst(dst_fname.toStdString(), CV_FOURCC_DEFAULT, fps, Size(src.get(CV_CAP_PROP_FRAME_WIDTH), src.get(CV_CAP_PROP_FRAME_HEIGHT))); // toLocal8Bit() почему-то выбрасывает segfault в Windows

		throw_if(! dst.isOpened(), "TODO");

		CDisplay::reset(25);
		measure.reset();

		for(counter = 0; __is_run && src.read(src_frame); counter++)
		{
			clock_gettime(CLOCK_REALTIME, & ts_before);

			throw_null(dst_frame = _lua.run(src_frame), "TODO");

			clock_gettime(CLOCK_REALTIME, & ts_after);

			measure(ts_after.tv_sec - ts_before.tv_sec + (ts_after.tv_nsec - ts_before.tv_nsec) / 1000000000.0);

			dst.write(* dst_frame);

			CDisplay::show_Mat(src_frame, * dst_frame);
			
			CImage::clear();

			if(counter == 10)
				break; // TODO
		}

		stop();
	}
	catch(...)
	{
		CImage::clear();

		throw_("TODO");
	}
}

void CMainLoop::start()
{
	__is_run = true;
	emit __start();
}

void CMainLoop::stop()
{
	__is_run = false;
	emit __stop();
}

