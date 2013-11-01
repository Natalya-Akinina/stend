
#include "config.hpp"

QString CConfig::_modules_dname = "", CConfig::_R_scripts_dname = "";

void CConfig::load(const QString fname)
{
    bool is_catalogs = false;
    QXmlStreamReader::TokenType type;
	QStringRef name;
	QXmlStreamReader stream;
	QFile fl(fname);

	throw_if(! fl.open(QIODevice::ReadOnly | QIODevice::Text), "TODO");
	stream.setDevice(& fl);

	while(stream.readNextStartElement())
		while(! stream.atEnd())
		{
			type = stream.readNext();
			name = stream.name();

			if(name == "catalogs")
            {
                switch(type)
                {
                    case QXmlStreamReader::StartElement:
                    {
                        is_catalogs = true;

                        break;
                    }
                    case QXmlStreamReader::EndElement:
                    {
                        is_catalogs = false;

                        break;
                    }
                }
            }
            else if(is_catalogs)
            {
                if(name == "modules")
                    CConfig::_modules_dname = stream.readElementText();
                else if(name == "R_scripts")
                    CConfig::_R_scripts_dname = stream.readElementText();
            }
		}
}

const QString CConfig::module_fname(const QString module_name)
{
    return CConfig::_modules_dname + "/lib" + module_name;
}

const QString CConfig::R_script_fname(const QString script_name)
{
    return CConfig::_R_scripts_dname + "/" + script_name + ".r";
}
