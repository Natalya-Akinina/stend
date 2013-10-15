
#ifndef MODULE_HPP
#define MODULE_HPP

#include "all.hpp"
#include "lib/interface.hpp"

typedef int (* init_function)(char *, const unsigned, unsigned *, unsigned *);
typedef int (* destroy_function)();
typedef int (* get_name_function)(const bool, const unsigned, char *, const unsigned);
typedef int (* get_type_function)(const bool, const unsigned, e_type *);
typedef int (* get_value_function)(const bool, const unsigned, void *);
typedef int (* set_value_function)(const bool, const unsigned, const void *);
typedef int (* run_function)();

typedef pair<QString, e_type> t_elem;

class CModule
{
	const unsigned buf_size = 4096;
	vector<t_elem> _params, _return_values;
	QString _name;
	QLibrary library;

	init_function _init;
	destroy_function _destroy;
	get_name_function _get_name;
	get_type_function _get_type;
	get_value_function _get_value;
	set_value_function _set_value;
	run_function _run;

	void load_elem(const bool is_param, const unsigned num);

	public:

		CModule(const QString fname);
		~CModule();

		inline const QString fname() { return library.fileName(); };
		inline const QString name() { return _name; };
		int run(lua_State * state);
};

#endif

