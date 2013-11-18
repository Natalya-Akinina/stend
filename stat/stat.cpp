
#include "stat/stat.hpp"

CMeasure::CMeasure()
{
	;
}

CMeasure::~CMeasure()
{
	;
}

void CMeasure::reset()
{
	__sec_per_frame.clear();
}

void CMeasure::operator()(const double sec_per_frame)
{
	__sec_per_frame.append(sec_per_frame);
}

// ############################################################################ 

CStat::CStat(CMeasure & measure) :
	__measure(measure)
{
	;
}

CStat::~CStat()
{
	;
}

void CStat::display()
{
	try
	{
		QProcess process;
		QStringList args;
		QTemporaryFile data_fl;
		QTemporaryFile png_fl("XXXXXX.png");
		QTemporaryFile tmp_script_fl("XXXXXX.r");
		QFile script_fl(":/scripts/" + name_en());

		throw_if(! data_fl.open(), "Не удалось создать временный файл для данных");
		save_data(data_fl);
		throw_if(! data_fl.flush(), "Не удалось сохранить временный файл для данных");

		throw_if(! tmp_script_fl.open(), "Не удалось создать временный файл для модуля");
		throw_if(! script_fl.open(QIODevice::ReadOnly | QIODevice::Text), "Не удалось открыть скрипт");

		const QByteArray data = script_fl.readAll();

		throw_if(data.isEmpty(), "Не удалось прочитать скрипт");
		throw_if(tmp_script_fl.write(data) == -1, "Не удалось записать данные во временный файл для модуля");
		throw_if(! tmp_script_fl.flush(), "Не удалось сохранить данные во временном файле для модуля");

		throw_if(! png_fl.open(), "Не удалось создать временный файл для графика");

		args << tmp_script_fl.fileName() << data_fl.fileName() << png_fl.fileName();

		process.start("Rscript", args);
		process.waitForFinished();
		throw_if(process.exitCode(), "Интерпретатор завершился с ошибкой");

		png_fl.flush();
		CDisplay::show_png(name_ru(), png_fl.fileName());
	}
	catch(...)
	{
		;
	}
}

void CStat::save(const QString fname)
{
	try
	{
		QString real_fname = fname;

		if(real_fname == "")
			real_fname = CDisplay::get_fname_to_save_stat();

		QFile fl(real_fname);

		throw_if(! fl.open(QIODevice::WriteOnly | QIODevice::Text), "Не удалось открыть результирующий файл");
		save_data(fl);
	}
	catch(...)
	{
		;
	}
}

// ############################################################################ 
// Секунд на кадр

CSecPerFrameStat::CSecPerFrameStat(CMeasure & measure) :
	CStat(measure)
{
	;
}

CSecPerFrameStat::~CSecPerFrameStat()
{
	;
}

void CSecPerFrameStat::save_data(QFile & fl)
{
	const unsigned frame_num = __measure.__sec_per_frame.size();
	unsigned v;
	QTextStream stream;

	stream.setDevice(& fl);
	stream.setRealNumberPrecision(11);
	stream << "frame" << "\t" << "sec_per_frame" << endl;

	for(v = 0; v < frame_num; v++)
		stream << v << "\t" << __measure.__sec_per_frame[v] << endl;
}

