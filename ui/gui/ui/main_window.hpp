
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/module.hpp"
#include "lib/matrix.hpp"
#include "lib/image.hpp"
#include "main_loop/main_loop.hpp"
#include "ui/gui/ui/about_dialog.hpp"
#include "ui_main_window.h"
#include "ui/gui/ui/lua_highlighter.hpp"

class CMainWindow : public QMainWindow, private Ui::main_window
{
	Q_OBJECT

	const QString script_window_title = trUtf8("Скрипт");
	const QString modules_list_window_title = trUtf8("Модули");
	const QString src_video_window_title = trUtf8("Исходная видеопоследовательность");
	const QString dst_video_window_title = trUtf8("Результирующая видеопоследовательность");
	QTextBrowser script_view;
	QListWidget modules_list;
	QLabel src_video_view, dst_video_view;
	QString script_fname, src_video_fname, dst_video_fname;
	QMenu process_menu, video_menu, foreground_menu, window_menu, stat_menu, stat_menu_display, stat_menu_save;
	QToolButton process_button, window_button, stat_button;
	QTextDocument script_document;
	QLuaHighlighter script_highlighter;
	QMdiSubWindow * script_window, * modules_list_window, * src_video_window, * dst_video_window;
	QAction * src_video_action, * dst_video_action, * script_action, * toggle_experiment_action;
	CLua lua;
	CMainLoop main_loop;
	
	void display(QMdiSubWindow * window, const Mat & img);

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
		~CMainWindow();

		void display(const Mat & src, const Mat & dst);

	public slots:

		void start_experiment();
		void stop_experiment();
	
		void about();
		void toggle_experiment();

		void foreground_script_window();
		void foreground_modules_list_window();
		void foreground_src_video_window();
		void foreground_dst_video_window();

		void process_script();
		void process_module();
		void modules_list_context_menu(const QPoint & pos);
		void process_src_video();
		void process_dst_video();
};

extern CMainWindow * p_main_window;

#endif

