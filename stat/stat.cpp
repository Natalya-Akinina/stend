
#include "stat/stat.hpp"

CStat::CStat(CLua & lua) :
	_lua(lua)
{
	;
}

void CStat::run(const QString src_fname, const QString dst_fname)
{
	unsigned counter;
	const string src_wname = "Source", dst_wname = "Result";
	struct timespec ts_before, ts_after;
	Mat src_frame, * dst_frame;
	VideoCapture src(src_fname.toStdString());

	throw_if(! src.isOpened());

	const double fps = src.get(CV_CAP_PROP_FPS);
	VideoWriter dst(dst_fname.toStdString(), CV_FOURCC_DEFAULT, fps, Size(src.get(CV_CAP_PROP_FRAME_WIDTH), src.get(CV_CAP_PROP_FRAME_HEIGHT))); // TODO Не работает
	throw_if(! dst.isOpened());

	namedWindow(src_wname, CV_WINDOW_NORMAL);
	namedWindow(dst_wname, CV_WINDOW_NORMAL);

	sec_per_frame.clear();

	for(counter = 0; src.read(src_frame); counter++)
	{
		clock_gettime(CLOCK_REALTIME, & ts_before);

		throw_null(dst_frame = _lua.run(src_frame));

		clock_gettime(CLOCK_REALTIME, & ts_after);

		if(counter >= 1) // TODO
			sec_per_frame.push_back(ts_after.tv_sec - ts_before.tv_sec + (ts_after.tv_nsec - ts_before.tv_nsec) / 1000000000.0);

		dst.write(* dst_frame);

		imshow(src_wname, src_frame);
		imshow(dst_wname, * dst_frame);

		waitKey(1000 / fps);

		CImage::clear();

		printf_TODO("%u\n", counter);
		//if(counter > 100) // TODO убрать
		//	break; // TODO убрать
	}
}

void CStat::display_sec_per_frame()
{
	QProcess process;
	QStringList args;
	const char * fname = tmpnam(NULL);

	save_sec_per_frame(fname);

	args << CConfig::R_script_fname("sec_per_frame") << fname;

	process.start("Rscript", args);
	process.waitForFinished();

	remove(fname);
}

void CStat::save_sec_per_frame(const QString fname)
{
	const unsigned frame_num = sec_per_frame.size();
	unsigned v;
	QTextStream stream;
	QFile fl(fname);

	throw_if(! fl.open(QIODevice::WriteOnly | QIODevice::Text));
	stream.setDevice(& fl);

	stream.setRealNumberPrecision(11);

	for(v = 0; v < frame_num; v++)
		stream << v << "\t" << sec_per_frame[v] << endl;
}

