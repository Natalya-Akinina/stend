
#ifndef LUA_HPP
#define LUA_HPP

#include "all.hpp"
#include "lib/module.hpp"
#include "lib/opencv.hpp"
#include "lib/matrix.hpp"
#include "lib/image.hpp"

class CLua
{
	map<QString, shared_ptr<CModule> > modules;

	public:

		lua_State * state;

		CLua();
		~CLua();
		
		QString load_module(const QString module_fname);
        void unload_module(const QString module_name);
		static int run_module(lua_State * state);
		void load_script(const QString fname);
		Mat * run(const Mat & src);
};

#endif

