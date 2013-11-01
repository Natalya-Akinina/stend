
#include "ui/gui/ui/main_window.hpp"

CMainWindow::CMainWindow() :
	QMainWindow(NULL), stat(lua)
{
	setupUi(this);

	// ############################################################################ 
	// Загрузка и сохранение

	process_video_menu.setTitle(trUtf8("Видео"));
	process_video_menu.setIcon(QIcon(":/icons/process_video"));
	process_src_video_action = process_video_menu.addAction(QIcon(":/icons/process_src_video"), "", this, SLOT(process_src_video())); // TODO
	process_dst_video_action = process_video_menu.addAction(QIcon(":/icons/save"), "", this, SLOT(process_dst_video()));

	process_script_action = process_menu.addAction(QIcon(":/icons/process_script"), "", this, SLOT(process_lua()));
	process_menu.addAction(QIcon(":/icons/process_module"), trUtf8("Загрузить модуль"), this, SLOT(process_module()));
	process_menu.addMenu(& process_video_menu);

	// ############################################################################ 
	// Статистика

	stat_menu.addAction(QIcon(":/icons/stat_full_time"), trUtf8("Время выполнения алгоритма на кадре"), this, SLOT(stat_full_time(bool)))->setCheckable(true);

	// ############################################################################ 

	auto add_action = [ this ](const QString title, const QString icon_name, const char * fun_name)
	{
		tool_bar->addAction(QIcon(icon_name), title, this, fun_name)->setToolTip(title);
	};

	auto add_button = [ this ](QToolButton & button, QMenu & menu, const QString title, const QString icon_name)
	{
		button.setToolTip(title);
		button.setIcon(QIcon(icon_name));
		button.setMenu(& menu);
		button.setPopupMode(QToolButton::InstantPopup);

		tool_bar->addWidget(& button);
	};

	add_button(process_button, process_menu, trUtf8("Загрузка и сохранение"), ":/icons/process");
	add_button(stat_button, stat_menu, trUtf8("Статистика"), ":/icons/stat");
	toggle_experiment_action = tool_bar->addAction("", this, SLOT(toggle_experiment()));
	add_action(trUtf8("О программе"), ":/icons/about", SLOT(about()));

	// ############################################################################ 

	unload_script();
	unload_src_video();
	unset_dst_video();
}

// ############################################################################ 
// Служебные функции

void CMainWindow::start_experiment()
{
	try
	{
		if(! is_experiment_run)
		{
			lua.load_script(script_fname);

			// TODO
			// stat.run(argv[3], argv[4]);
			// stat.display_sec_per_frame();
		}
		
		is_experiment_run = true;

		toggle_experiment_action->setIcon(QIcon(":/icons/stop"));
		toggle_experiment_action->setToolTip(trUtf8("Останов эксперимента"));
	}
	catch(...)
	{
		;
	}
}

void CMainWindow::stop_experiment()
{
	if(is_experiment_run)
	{
		// TODO
		;
	}

	is_experiment_run = false;

	toggle_experiment_action->setIcon(QIcon(":/icons/start"));
	toggle_experiment_action->setToolTip(trUtf8("Запуск эксперимента"));
	toggle_experiment_action->setEnabled(! (script_fname.isEmpty() || src_video_fname.isEmpty() || dst_video_fname.isEmpty()));
}

void CMainWindow::load_script(const QString __script_fname)
{
	try
	{
		throw_if(__script_fname.isEmpty(), NULL)

		QFile fl(__script_fname);

		throw_if(! fl.open(QIODevice::ReadOnly | QIODevice::Text), "Невозможно открыть файл");

		lua_script.setPlainText(fl.readAll());
		lua_script_highlighter.setDocument(& lua_script);
		lua_script_view->setDocument(& lua_script);
		lua_script_view->setTabStopWidth(10);

		process_script_action->setText(script_action_text_prefix + trUtf8(" - ") + __script_fname);
		script_fname = __script_fname;

		stop_experiment();
	}
	catch(...)
	{
		unload_script();
	}
}

void CMainWindow::unload_script()
{
	process_script_action->setText(script_action_text_prefix);
	lua_script_view->clear();

	stop_experiment();
}

void CMainWindow::load_module(const QString module_fname)
{
	const QString module_name = lua.load_module(module_fname);

	if(modules_list->findItems(module_name, Qt::MatchExactly).size() == 0)
		modules_list->addItem(module_name);
}

void CMainWindow::unload_module(QListWidgetItem * current_item)
{
	lua.unload_module(current_item->text());
	delete current_item;
}

void CMainWindow::load_src_video(const QString video_fname)
{
	try
	{
		throw_if(video_fname.isEmpty(), NULL)

		// TODO

		process_src_video_action->setText(src_video_action_text_prefix + trUtf8(" - ") + video_fname);
		src_video_fname = video_fname;

		stop_experiment();
	}
	catch(...)
	{
		unload_src_video();
	}
}

void CMainWindow::unload_src_video()
{
	process_src_video_action->setText(src_video_action_text_prefix);

	// TODO

	stop_experiment();
}

void CMainWindow::set_dst_video(const QString video_fname)
{
	try
	{
		throw_if(video_fname.isEmpty(), NULL)

		// TODO

		process_dst_video_action->setText(dst_video_action_text_prefix + trUtf8(" - ") + video_fname);
		dst_video_fname = video_fname;

		stop_experiment();
	}
	catch(...)
	{
		unset_dst_video();
	}
}

void CMainWindow::unset_dst_video()
{
	process_dst_video_action->setText(dst_video_action_text_prefix);

	// TODO

	stop_experiment();
}

// ############################################################################ 
// Слоты

void CMainWindow::about()
{
	try
	{
		CAboutDialog(this).exec();
	}
	catch(...)
	{
		;
	}
}

// ############################################################################ 
// Слоты - загрузка и выгрузка исходных данных

void CMainWindow::process_lua()
{
	try
	{
		load_script(
				QFileDialog::getOpenFileName(this, trUtf8("Загрузить Lua-скрипт"), "", trUtf8("Lua-скрипт (*.lua)"))
				);
	}
	catch(...)
	{
		;
	}
}

void CMainWindow::process_module()
{
	try
	{
		const QString fname = QFileDialog::getOpenFileName(this, trUtf8("Загрузить модуль"), "", trUtf8("Модуль (*.so *.dll)"));

		throw_if(fname.isEmpty(), NULL);
		load_module(fname);
	}
	catch(...)
	{
		;
	}

	stop_experiment();
}

void CMainWindow::on_modules_list_customContextMenuRequested(const QPoint & pos)
{
	try
	{
		QListWidgetItem * current_item = modules_list->currentItem();

		if(current_item != NULL && current_item->isSelected())
		{
			QMenu context_menu;
			QAction * delete_action = context_menu.addAction(QIcon(":/icons/delete"), trUtf8("Выгрузить модуль"));

			if(delete_action == context_menu.exec(modules_list->mapToGlobal(pos)))
				unload_module(current_item);
		}
	}
	catch(...)
	{
		;
	}

	stop_experiment();
}

void CMainWindow::process_src_video()
{
	// TODO
	;
}

void CMainWindow::process_dst_video()
{
	// TODO
	;
}

// ############################################################################ 
// Слот - запуск эксперимента 

void CMainWindow::toggle_experiment()
{
	if(is_experiment_run)
		stop_experiment();
	else
		start_experiment();
}

// ############################################################################ 
// Слоты - статистика

void CMainWindow::stat_full_time(const bool is_checked)
{
	; // TODO
}

