
#include "ui/gui/ui/main_window.hpp"

CMainWindow::CMainWindow() :
	QMainWindow(NULL), script_view(this), modules_list(this), src_video_view(this), dst_video_view(this), main_loop(lua)
{
	setupUi(this);

	// ############################################################################ 
	// Связь с главным циклом и драйвером дисплея

	p_main_window = this;
	connect(& main_loop, SIGNAL(__start()), this, SLOT(start_experiment()));
	connect(& main_loop, SIGNAL(__stop()), this, SLOT(stop_experiment()));

	// ############################################################################ 
	// Настройка виджетов

	modules_list.setEditTriggers(QAbstractItemView::NoEditTriggers);
	modules_list.setSelectionMode(QAbstractItemView::SingleSelection);
	modules_list.setContextMenuPolicy(Qt::CustomContextMenu);
	connect(& modules_list, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(modules_list_context_menu(const QPoint &)));

	const auto flags = Qt::SubWindow | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowShadeButtonHint;

	setup_sub_window(script_window, & script_view, script_window_title, ":/icons/script", QSize(600, 600), flags, false);
	setup_sub_window(modules_list_window, & modules_list, modules_list_window_title, ":/icons/module", QSize(200, 400), flags, false);
	setup_sub_window(src_video_window, & src_video_view, src_video_window_title, ":/icons/src_video", QSize(400, 400), flags, false);
	setup_sub_window(dst_video_window, & dst_video_view, dst_video_window_title, ":/icons/save", QSize(400, 400), flags, false);

	modules_list_window->setStatusTip(modules_list_window_title);

	// ############################################################################ 
	// Загрузка и сохранение

	video_menu.setTitle(trUtf8("Видео"));
	video_menu.setIcon(QIcon(":/icons/video"));
	src_video_action = video_menu.addAction(QIcon(":/icons/src_video"), trUtf8("Загрузить исходное видео"), this, SLOT(process_src_video()));
	dst_video_action = video_menu.addAction(QIcon(":/icons/save"), trUtf8("Сохранить результирующее видео"), this, SLOT(process_dst_video()));

	script_action = process_menu.addAction(QIcon(":/icons/script"), trUtf8("Загрузить Lua - скрипт"), this, SLOT(process_script()));
	process_menu.addAction(QIcon(":/icons/module"), trUtf8("Загрузить модуль"), this, SLOT(process_module()));
	process_menu.addMenu(& video_menu);

	// ############################################################################ 
	// Организация окон

	foreground_menu.setTitle(trUtf8("На передний план"));
	foreground_menu.setIcon(QIcon(":/icons/foreground"));
	foreground_menu.addAction(QIcon(":/icons/script"), script_window_title, this, SLOT(foreground_script_window()));
	foreground_menu.addAction(QIcon(":/icons/module"), modules_list_window_title, this, SLOT(foreground_modules_list_window()));
	foreground_menu.addAction(QIcon(":/icons/src_video"), src_video_window_title, this, SLOT(foreground_src_video_window()));
	foreground_menu.addAction(QIcon(":/icons/dst_video"), dst_video_window_title, this, SLOT(foreground_dst_video_window()));

	window_menu.addMenu(& foreground_menu);
	window_menu.addAction(QIcon(":/icons/cascade"), trUtf8("Каскадом"), mdi_area, SLOT(cascadeSubWindows()));
	window_menu.addAction(QIcon(":/icons/tile"), trUtf8("Равномерно распределить"), mdi_area, SLOT(tileSubWindows()));

	// ############################################################################ 
	// Статистика

	stat_menu_display.setTitle(trUtf8("Вывести на экран"));
	stat_menu_display.setIcon(QIcon(":/stat/display"));

	stat_menu_save.setTitle(trUtf8("Сохранить"));
	stat_menu_save.setIcon(QIcon(":/stat/save"));

	for(auto & it : main_loop.stats)
	{
		QString icon_name = ":/stat/";
		CStat * obj = it.get();
		icon_name += obj->name_en();

		stat_menu_display.addAction(QIcon(icon_name), obj->name_ru(), obj, SLOT(display()));
		stat_menu_save.addAction(QIcon(icon_name), obj->name_ru(), obj, SLOT(save()));
	}

	stat_menu.addMenu(& stat_menu_display);
	stat_menu.addMenu(& stat_menu_save);
	stat_menu.setEnabled(false);

	// ############################################################################ 

	auto add_action = [ this ](const QString title, const QString icon_name, const char * fun_name)
	{
		QAction * action = tool_bar->addAction(QIcon(icon_name), title, this, fun_name);
		
		action->setToolTip(title);
		action->setStatusTip(title);
	};

	auto add_button = [ this ](QToolButton & button, QMenu & menu, const QString title, const QString icon_name)
	{
		button.setToolTip(title);
		button.setStatusTip(title);
		button.setIcon(QIcon(icon_name));
		button.setMenu(& menu);
		button.setPopupMode(QToolButton::InstantPopup);

		tool_bar->addWidget(& button);
	};

	add_button(process_button, process_menu, trUtf8("Загрузка и сохранение"), ":/icons/process");
	add_button(window_button, window_menu, trUtf8("Управление окнами"), ":/icons/window");
	add_button(stat_button, stat_menu, trUtf8("Статистика"), ":/stat/stat");
	toggle_experiment_action = tool_bar->addAction("", this, SLOT(toggle_experiment()));
	add_action(trUtf8("О программе"), ":/icons/about", SLOT(about()));

	// ############################################################################ 

	unload_script();
	modules_list.clear();
	unload_src_video();
	unset_dst_video();
}

CMainWindow::~CMainWindow()
{
	;
}

void CMainWindow::setup_sub_window(QMdiSubWindow *& window, QWidget * widget, const QString window_title, const QString icon_name, const QSize size, const QFlags<Qt::WindowType> flags, bool delete_on_close)
{
	throw_null(window = mdi_area->addSubWindow(widget, flags), "Не удалось создать дочернее окно");
	window->setWindowTitle(window_title);
	window->setWindowIcon(QIcon(icon_name)); // TODO Из-за флагов иконка не отображается
	window->resize(size);
	window->setAttribute(Qt::WA_DeleteOnClose, delete_on_close);
	window->setSystemMenu(NULL);
	mdi_area->setActiveSubWindow(window);
	window->show();
}

QString CMainWindow::get_fname_to_save_stat()
{
	return QFileDialog::getSaveFileName(this, trUtf8("Сохранить статистику"), "", trUtf8("Текстовый файл (*.txt)"));
}

void CMainWindow::display_Mat(QMdiSubWindow * window, const Mat & img)
{
	Mat rgb;
	const QSize size(img.cols, img.rows);

	cvtColor(img, rgb, CV_BGR2RGB);

	((QLabel *) window->widget())->setPixmap
	(
		QPixmap::fromImage
		(
			QImage((const unsigned char *) rgb.data, rgb.cols, rgb.rows, QImage::Format_RGB888)
		)
	);

	window->resize(size);
	window->setMaximumSize(size);
}

void CMainWindow::display_Mat(const Mat & src, const Mat & dst)
{
	display_Mat(src_video_window, src);
	display_Mat(dst_video_window, dst);
}

void CMainWindow::display_png(const QString & name_ru, const QString & fname)
{
	QMdiSubWindow * window = NULL;
	QLabel * view = NULL;
	QPixmap pixmap(fname);
	QSize size;

	try
	{
		throw_if(pixmap.isNull(), "Не удалось загрузить график");
		throw_null(view = new QLabel(this), "Не удалось создать средство отображения PNG-файлов");
		view->setPixmap(pixmap);
		size = pixmap.size();
		setup_sub_window(window, view, name_ru, ":/icons/experiment_results", size, 0, true);
		window->setMaximumSize(size);
	}
	catch(...)
	{
		if(window != NULL)
			delete window;
		else if(view != NULL) // We bind view to window using DeleteOnClose
			delete view;
	}
}

// ############################################################################ 
// Контроль эксперимента

void CMainWindow::start_experiment()
{
	try
	{
		const QString tip = trUtf8("Останов эксперимента");

		toggle_experiment_action->setIcon(QIcon(":/icons/stop"));
		toggle_experiment_action->setToolTip(tip);
		toggle_experiment_action->setStatusTip(tip);

		stat_menu.setEnabled(true);
	}
	catch(...)
	{
		;
	}
}

void CMainWindow::stop_experiment()
{
	const QString tip = trUtf8("Запуск эксперимента");
	const bool is_experiment_may_be_run = ! (script_fname.isEmpty() || src_video_fname.isEmpty() || dst_video_fname.isEmpty());

	toggle_experiment_action->setIcon(QIcon(":/icons/start"));
	toggle_experiment_action->setToolTip(tip);
	toggle_experiment_action->setStatusTip(tip);
	toggle_experiment_action->setEnabled(is_experiment_may_be_run);

	// TODO Должен вызываться main_loop.stop() (чтобы цикл в main_loop остановился)
}

void CMainWindow::toggle_experiment()
{
	if(main_loop.is_run())
		main_loop.stop();
	else
	{
		lua.load_script(script_fname);
		main_loop.start(src_video_fname, dst_video_fname);
	}
}

// ############################################################################ 
// Окна на передний план

#define FOREGROUND(fun, window)\
void CMainWindow::fun()\
{\
	mdi_area->setActiveSubWindow(window);\
}

FOREGROUND(foreground_script_window, script_window)
FOREGROUND(foreground_modules_list_window, modules_list_window)
FOREGROUND(foreground_src_video_window, src_video_window)
FOREGROUND(foreground_dst_video_window, dst_video_window)

// ############################################################################ 
// Загрузка, установ, выгрузка и сброс

#define LOAD_SET(fun, unload_fun, fname, window, window_title, code)\
void CMainWindow::fun(const QString __fname)\
{\
	try\
	{\
		code;\
\
		window->setStatusTip(__fname);\
		fname = __fname;\
\
		stop_experiment();\
	}\
	catch(...)\
	{\
		unload_fun();\
	}\
}

#define UNLOAD_UNSET(fun, fname, window, content, window_title)\
void CMainWindow::fun()\
{\
	window->setStatusTip(window_title);\
	fname.clear();\
	content.clear();\
\
	stop_experiment();\
}

LOAD_SET(load_script, unload_script, script_fname, script_window, script_window_title,
	QFile fl(__fname);
	throw_if(! fl.open(QIODevice::ReadOnly | QIODevice::Text), "Невозможно открыть файл");

	script_document.setPlainText(fl.readAll());
	script_highlighter.setDocument(& script_document);
	script_view.setDocument(& script_document);
	script_view.setTabStopWidth(10);
);

LOAD_SET(load_src_video, unload_src_video, src_video_fname, src_video_window, src_video_window_title,
	Mat frame;
	VideoCapture video(__fname.toLocal8Bit().constData());

	throw_if(! video.isOpened(), "Не удалось открыть файл с исходной видеопоследовательностью");
	video.set(CV_CAP_PROP_POS_FRAMES, video.get(CV_CAP_PROP_FRAME_COUNT) / 2);
	throw_if(! video.read(frame), "Не удалось прочитать файл с исходной видеопоследовательностью");
	display_Mat(src_video_window, frame);
);

LOAD_SET(set_dst_video, unset_dst_video, dst_video_fname, dst_video_window, dst_video_window_title,
	const QPixmap img = QPixmap(":/icons/yes");
	const QSize size = img.size();

	dst_video_view.setPixmap(img);
	dst_video_window->resize(size);
	dst_video_window->setMaximumSize(size);
);


UNLOAD_UNSET(unload_script, script_fname, script_window, script_view, script_window_title)
UNLOAD_UNSET(unload_src_video, src_video_fname, src_video_window, src_video_view, src_video_window_title)
UNLOAD_UNSET(unset_dst_video, dst_video_fname, dst_video_window, dst_video_view, dst_video_window_title)

void CMainWindow::load_module(const QString module_fname)
{
	const QString module_name = lua.load_module(module_fname);

	if(! modules_list.findItems(module_name, Qt::MatchExactly).size())
		modules_list.addItem(module_name);
}

void CMainWindow::unload_module(QListWidgetItem * current_item)
{
	lua.unload_module(current_item->text());
	delete current_item;
}

// ############################################################################ 
// Слоты - загрузка и выгрузка исходных данных

#define PROCESS(msg, suffix, fun, load_fun, open_save_fun)\
void CMainWindow::fun()\
{\
	try\
	{\
		const QString fname = QFileDialog::open_save_fun(this, trUtf8(msg), "", trUtf8(suffix));\
\
		if(! fname.isEmpty())\
			load_fun(fname);\
	}\
	catch(...)\
	{\
		;\
	}\
}

PROCESS("Загрузить Lua-скрипт", "Lua-скрипт (*.lua)", process_script, load_script, getOpenFileName)
PROCESS("Загрузить модуль", "Модуль (*.so *.dll)", process_module, load_module, getOpenFileName)
PROCESS("Загрузить исходную видеопоследовательность", "Видео (*.avi *.mpg *.mp4);;Кадр (*.bmp *.jpg *.png *.tiff)", process_src_video, load_src_video, getOpenFileName) // TODO Более полное перечисление расширений
PROCESS("Сохранить результирующую видеопоследовательность", "Видео (*.avi *.mpg *.mp4);;Кадр (*.bmp *.jpg *.png *.tiff)", process_dst_video, set_dst_video, getSaveFileName) // TODO Более полное перечисление расширений + автодобавление расширения, если его нет 

void CMainWindow::modules_list_context_menu(const QPoint & pos)
{
	try
	{
		QListWidgetItem * current_item = modules_list.currentItem();

		if(current_item != NULL && current_item->isSelected())
		{
			QMenu context_menu;
			QAction * delete_action = context_menu.addAction(QIcon(":/icons/delete"), trUtf8("Выгрузить модуль"));

			if(delete_action == context_menu.exec(modules_list.mapToGlobal(pos)))
				unload_module(current_item);
		}
	}
	catch(...)
	{
		;
	}

	stop_experiment();
}

// ############################################################################ 
// Прочие слоты

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

