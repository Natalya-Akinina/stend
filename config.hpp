
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "all.hpp"

class CConfig
{
    static QString _modules_dname, _R_scripts_dname;

    public:

        virtual ~CConfig() = 0;

        static void load(const QString fname);
        static const QString module_fname(const QString module_name);
        static const QString R_script_fname(const QString script_name);
};

#endif
