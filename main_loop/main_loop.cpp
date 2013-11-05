
#include "main_loop/main_loop.hpp"

CMainLoop::CMainLoop(CLua & lua) :
	_lua(lua)
{
	;
}

CMainLoop::~CMainLoop()
{
	;
}

void CMainLoop::operator()(const QString src_fname, const QString dst_fname)
{
	try
	{
		unsigned counter;
		struct timespec ts_before, ts_after;
		Mat src_frame, * dst_frame;
		VideoCapture src(src_fname.toStdString());
		shared_ptr<CDisplay> disp(CDisplay::create(25));
		CDisplay * pdisp = disp.get();

		throw_null(pdisp, "Не удалось инициализировать дисплей");
		throw_if(! src.isOpened(), "TODO");

		const double fps = src.get(CV_CAP_PROP_FPS);
		VideoWriter dst(dst_fname.toStdString(), CV_FOURCC_DEFAULT, fps, Size(src.get(CV_CAP_PROP_FRAME_WIDTH), src.get(CV_CAP_PROP_FRAME_HEIGHT))); // TODO Не работает

		throw_if(! dst.isOpened(), "TODO");

		sec_per_frame.init();

		for(counter = 0; pdisp->is_run() && src.read(src_frame); counter++)
		{
			clock_gettime(CLOCK_REALTIME, & ts_before);

			throw_null(dst_frame = _lua.run(src_frame), "TODO");

			clock_gettime(CLOCK_REALTIME, & ts_after);

			sec_per_frame(ts_after.tv_sec - ts_before.tv_sec + (ts_after.tv_nsec - ts_before.tv_nsec) / 1000000000.0);

			dst.write(* dst_frame);

			(* pdisp)(src_frame, * dst_frame);
			
			CImage::clear();
		}

		pdisp->stop();
	}
	catch(...)
	{
		CImage::clear();

		throw_("TODO");
	}
}

