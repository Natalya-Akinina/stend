
#include "lib/lua.hpp"

CLua::CLua()
{
	throw_null(state = luaL_newstate(), "TODO");
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

QString CLua::load_module(const QString module_fname)
{
	CModule * p_module;

	for(auto & md : modules)
		if(md.second->fname() == module_fname)
			return md.second->name();

	throw_null(p_module = new CModule(module_fname), "TODO");
	const QString module_name = p_module->name();
	modules[module_name] = shared_ptr<CModule>(p_module);

	return module_name;
}

void CLua::unload_module(const QString module_name)
{
	qTODO("unload_module");
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
	throw_if(luaL_dofile(state, fname.toStdString().c_str()) != LUA_OK, "TODO");
}

Mat * CLua::run(const Mat & src)
{
	Mat * _dst;

	lua_getglobal(state, "main");
    lua_pushlightuserdata(state, image_copy(src));

	throw_if(lua_pcall(state, 1, 1, 0) != LUA_OK, "TODO");

	_dst = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
	lua_pop(state, 1);

	return _dst;
}

