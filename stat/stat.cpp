
#include "stat/stat.hpp"

CStat::CStat()
{
	;
}

CStat::~CStat()
{
	;
}

// ############################################################################ 
// Секунд на кадр

CSecPerFrameStat::CSecPerFrameStat() :
	CStat()
{
	;
}

CSecPerFrameStat::~CSecPerFrameStat()
{
	;
}

void CSecPerFrameStat::init()
{
	values.clear();
}

void CSecPerFrameStat::operator()(const double value)
{
	if(measure)
		values.push_back(value);
}

void CSecPerFrameStat::display()
{
	if(measure)
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
}

void CSecPerFrameStat::save(const QString fname)
{
	if(measure)
	{
		const unsigned frame_num = values.size();
		unsigned v;
		QTextStream stream;
		QFile fl(fname);

		throw_if(! fl.open(QIODevice::WriteOnly | QIODevice::Text), "TODO");
		stream.setDevice(& fl);

		stream.setRealNumberPrecision(11);

		for(v = 0; v < frame_num; v++)
			stream << v << "\t" << values[v] << endl;
	}
}

