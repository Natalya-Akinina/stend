
#include "lib/module.hpp"

CModule::CModule(const QString fname)
{
	unsigned param_num, return_value_num;
	char buf[buf_size];

	library.setFileName(fname);
	throw_if(! library.load(), "TODO");

	// ############################################################################ 
	// Функции

	throw_null(_init = (init_function) resolve("init"), "TODO");
	throw_null(_destroy = (destroy_function) resolve("destroy"), "TODO");
	throw_null(_get_name = (get_name_function) resolve("get_name"), "TODO");
	throw_null(_get_type = (get_type_function) resolve("get_type"), "TODO");
	throw_null(_get_value = (get_value_function) resolve("get_value"), "TODO");
	throw_null(_set_value = (set_value_function) resolve("set_value"), "TODO");
	throw_null(_run = (run_function) resolve("run"), "TODO");

	// ############################################################################ 
	// Переменные

#define LOAD_VARIABLE(var_name, fun_name, type)\
{\
	typedef type;\
	fun_type ptr;\
	throw_null(ptr = (fun_type) resolve(var_name), "TODO");\
	* ptr = & fun_name;\
}

	LOAD_VARIABLE("image_create", image_create, image (** fun_type)(const unsigned height, const unsigned width, const unsigned ch_num))
	LOAD_VARIABLE("image_delete", image_delete, int (** fun_type)(const image img))
	LOAD_VARIABLE("image_copy", image_copy, image (** fun_type)(const image img));
	LOAD_VARIABLE("matrix_to_image", matrix_to_image, image (** fun_type)(const matrix mtx));
	LOAD_VARIABLE("matrix_create", matrix_create, matrix (** fun_type)(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type));
	LOAD_VARIABLE("matrix_delete", matrix_delete, int (** fun_type)(matrix mtx))
	LOAD_VARIABLE("matrix_copy", matrix_copy, matrix (** fun_type)(matrix mtx))
	LOAD_VARIABLE("matrix_load_image", matrix_load_image, matrix (** fun_type)(const char * fname))
	LOAD_VARIABLE("matrix_save_image", matrix_save_image, int (** fun_type)(matrix mtx, const char * fname))
	LOAD_VARIABLE("matrix_get_value", matrix_get_value, int (** fun_type)(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, void * value))
	LOAD_VARIABLE("matrix_set_value", matrix_set_value, int (** fun_type)(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, const void * value))
	LOAD_VARIABLE("matrix_height", matrix_height, int (** fun_type)(matrix mtx, unsigned * value))
	LOAD_VARIABLE("matrix_width", matrix_width, int (** fun_type)(matrix mtx, unsigned * value))
	LOAD_VARIABLE("matrix_number_of_channels", matrix_number_of_channels, int (** fun_type)(matrix mtx, unsigned * value))
	LOAD_VARIABLE("matrix_element_type", matrix_element_type, int (** fun_type)(matrix mtx, int * value))
	LOAD_VARIABLE("matrix_pointer_to_data", matrix_pointer_to_data, int (** fun_type)(matrix mtx, void ** ptr));
	LOAD_VARIABLE("matrix_pointer_to_row", matrix_pointer_to_row, int (** fun_type)(matrix mtx, const unsigned row, void ** ptr));
	
	// ############################################################################ 

	throw_if((* _init)(buf, buf_size, & param_num, & return_value_num), "TODO");
	_name = buf;

	load_elem(true, param_num);
	load_elem(false, return_value_num);
}

CModule::~CModule()
{
	throw_if((* _destroy)(), "TODO");
}

QFunctionPointer CModule::resolve(const QString name)
{
	QFunctionPointer ptr = NULL;

	ptr = library.resolve(name.toLocal8Bit());

	// Если вдруг Borland'овский cdecl
	if(ptr == NULL)
		ptr = library.resolve((QString("_") + name).toLocal8Bit());

	throw_null(ptr, "TODO");

	return ptr;
}

void CModule::load_elem(const bool is_param, const unsigned num)
{
	unsigned v;
	char buf[buf_size];
	int type;
	auto & array = is_param ? _params : _return_values;

	for(v = 0; v < num; v++)
	{
		throw_if((* _get_name)(is_param, v, buf, buf_size), "TODO");
		throw_if((* _get_type)(is_param, v, & type), "TODO");

		array.append(QPair<QString, int>(QObject::trUtf8(buf), type));
	}
}

int CModule::run(lua_State * state)
{
	unsigned v;

	v = 2; // Первый параметр - имя алгоритма

	for(auto & param : _params)
	{
		switch(param.second)
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
			SET_PARAM(STRING_TYPE, char *, lua_tostring);
			SET_PARAM(BOOLEAN_TYPE, int, lua_toboolean);
			SET_PARAM(MATRIX_TYPE, matrix, lua_touserdata);
			SET_PARAM(IMAGE_TYPE, image, lua_touserdata);
			SET_PARAM(VOID_POINTER_TYPE, void *, lua_touserdata);
		}

		v++;
	}

	throw_if((* _run)(), "TODO");

	v = 0;

	for(auto & return_value : _return_values)
	{
		switch(return_value.second)
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
			GET_RETURN_VALUES(STRING_TYPE, char *, lua_pushstring);
			GET_RETURN_VALUES(BOOLEAN_TYPE, int, lua_pushboolean);
			GET_RETURN_VALUES(MATRIX_TYPE, matrix, lua_pushlightuserdata);
			GET_RETURN_VALUES(IMAGE_TYPE, image, lua_pushlightuserdata);
			GET_RETURN_VALUES(VOID_POINTER_TYPE, void *, lua_pushlightuserdata);
		}

		v++;
	}

	return _return_values.size();
}

