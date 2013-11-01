
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/module.hpp"
#include "lib/matrix.hpp"
#include "lib/image.hpp"
#include "stat/stat.hpp"
#include "ui/gui/ui/about_dialog.hpp"
#include "ui_main_window.h"
#include "ui/gui/ui/lua_highlighter.hpp"

class CMainWindow : public QMainWindow, private Ui::main_window
{
	Q_OBJECT

	bool is_experiment_run = false;
	const QString script_action_text_prefix = trUtf8("Загрузить Lua - скрипт");
	const QString src_video_action_text_prefix = trUtf8("Загрузить исходное видео");
	const QString dst_video_action_text_prefix = trUtf8("Сохранить результирующее видео");
	QString script_fname, src_video_fname, dst_video_fname;
	QMenu process_menu, process_video_menu, stat_menu;
	QToolButton process_button, stat_button;
	QTextDocument lua_script;
	QLuaHighlighter lua_script_highlighter;
	QAction * process_src_video_action, * process_dst_video_action, * process_script_action, * toggle_experiment_action;
	CLua lua;
	CStat stat;
	
	void start_experiment();
	void stop_experiment();
	
	void load_module(const QString module_fname);
	void unload_module(QListWidgetItem * current_item);

	void load_script(const QString script_fname);
	void unload_script();

	void load_src_video(const QString video_fname);
	void unload_src_video();

	void set_dst_video(const QString video_fname);
	void unset_dst_video();

	public:

		CMainWindow();

	public slots:

		void about();

		void process_lua();
		void process_module();
		void on_modules_list_customContextMenuRequested(const QPoint & pos);
		void process_src_video();
		void process_dst_video();

		void toggle_experiment();

		void stat_full_time(const bool is_checked);
};

#endif

