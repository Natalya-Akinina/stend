
#include "lib/module.hpp"

CModule::CModule(const QString fname)
{
	unsigned param_num, return_value_num;
	char buf[buf_size];

	library.setFileName(fname);
	library.load();

	throw_null(_init = (init_function) library.resolve("init"));
	throw_null(_destroy = (destroy_function) library.resolve("destroy"));
	throw_null(_get_name = (get_name_function) library.resolve("get_name"));
	throw_null(_get_type = (get_type_function) library.resolve("get_type"));
	throw_null(_get_value = (get_value_function) library.resolve("get_value"));
	throw_null(_set_value = (set_value_function) library.resolve("set_value"));
	throw_null(_run = (run_function) library.resolve("run"));

	throw_if((* _init)(buf, buf_size, & param_num, & return_value_num));
	_name = buf;

	load_elem(true, param_num);
	load_elem(false, return_value_num);
}

CModule::~CModule()
{
	throw_if((* _destroy)());
}

void CModule::load_elem(const bool is_param, const unsigned num)
{
	unsigned v;
	char buf[buf_size];
	e_type type;
	vector<t_elem> & array = is_param ? _params : _return_values;

	for(v = 0; v < num; v++)
	{
		throw_if((* _get_name)(is_param, v, buf, buf_size));
		throw_if((* _get_type)(is_param, v, & type));

		array.push_back(t_elem(buf, type));
	}
}

int CModule::run(lua_State * state)
{
	unsigned v;

	v = 2; // Первый параметр - имя алгоритма

	for(auto & it : _params)
	{
		switch(it.second)
		{

#define SET_PARAM(MY_TYPE, CPP_TYPE, LUA_FUN)\
case MY_TYPE:\
{\
	CPP_TYPE value = (CPP_TYPE) LUA_FUN(state, v);\
	(* _set_value)(true, v - 2, & value);\
\
	break;\
}

			SET_PARAM(INT_TYPE, int, lua_tointeger);
			SET_PARAM(DOUBLE_TYPE, double, lua_tonumber);
			SET_PARAM(IMAGE_TYPE, s_image *, lua_touserdata);
		}

		v++;
	}

	throw_if((* _run)());

	v = 0;

	for(auto & it : _return_values)
	{
		switch(it.second)
		{

#define GET_RETURN_VALUES(MY_TYPE, CPP_TYPE, LUA_FUN)\
case MY_TYPE:\
{\
	CPP_TYPE value;\
\
	(* _get_value)(false, v, & value);\
	LUA_FUN(state, value);\
\
	break;\
}

			GET_RETURN_VALUES(INT_TYPE, int, lua_pushinteger);
			GET_RETURN_VALUES(DOUBLE_TYPE, double, lua_pushnumber);
			GET_RETURN_VALUES(IMAGE_TYPE, s_image *, lua_pushlightuserdata);
		}

		v++;
	}

	return _return_values.size();
}

