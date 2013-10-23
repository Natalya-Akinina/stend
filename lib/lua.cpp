
#include "lib/lua.hpp"

CLua::CLua()
{
	throw_null(state = luaL_newstate());
	luaL_openlibs(state);

	opencv_register(state); // Регистрация биндингов к OpenCV

	// ############################################################################
	// Регистрация функции - обработчика доступа к модулям

	lua_pushlightuserdata(state, this);
	lua_pushcclosure(state, CLua::run_module, 1);
	lua_setglobal(state, "algo");
}

CLua::~CLua()
{
	lua_close(state);
}

void CLua::load_module(const QString module_name)
{
	const QString fname = CConfig::module_fname(module_name);
	CModule * p_module;

	for(auto & md : modules)
		if(md.second->fname() == fname)
			return;

	throw_null(p_module = new CModule(fname));
	modules[p_module->name()] = shared_ptr<CModule>(p_module);
}

int CLua::run_module(lua_State * state)
{
	QString name = (const char *) lua_tostring(state, 1);
	CLua * _this = (CLua *) lua_touserdata(state, lua_upvalueindex(1));
	auto md = _this->modules[name];

	return (md ? md->run(state) : 1);
}

void CLua::load_script(const QString fname)
{
	throw_if(luaL_dofile(state, fname.toStdString().c_str()) != LUA_OK);
}

Mat * CLua::run(const Mat & src)
{
	Mat * _dst;

	lua_getglobal(state, "main");
    lua_pushlightuserdata(state, image_copy(src));

	throw_if(lua_pcall(state, 1, 1, 0) != LUA_OK);

	_dst = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
	lua_pop(state, 1);

	return _dst;
}

