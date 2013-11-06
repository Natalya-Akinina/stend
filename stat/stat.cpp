
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

void CSecPerFrameStat::display()
{
	try
	{
		QProcess process;
		QStringList args;
		const char * fname = tmpnam(NULL);

		save(fname);

		args << CConfig::R_script_fname("sec_per_frame") << fname;

		process.start("Rscript", args);
		process.waitForFinished();
		
		remove(fname);
	}
	catch(...)
	{
		;
	}
}

void CSecPerFrameStat::save(const QString fname)
{
	try
	{
		// TODO получить fname, если fname == ""

		const unsigned frame_num = __measure.__sec_per_frame.size();
		unsigned v;
		QTextStream stream;
		QFile fl(fname);

		throw_if(! fl.open(QIODevice::WriteOnly | QIODevice::Text), "Ошибка при открытии файла");
		stream.setDevice(& fl);

		stream.setRealNumberPrecision(11);

		for(v = 0; v < frame_num; v++)
			stream << v << "\t" << __measure.__sec_per_frame[v] << endl;
	}
	catch(...)
	{
		;
	}
}

