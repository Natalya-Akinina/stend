
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "all.hpp"
#include "ui/gui/ui/about_dialog.hpp"
#include "ui_main_window.h"

class CMainWindow : public QMainWindow, private Ui::main_window
{
	Q_OBJECT

	public:

		CMainWindow();

	public slots:

		void on_about_action_triggered(bool checked);
};

#endif

