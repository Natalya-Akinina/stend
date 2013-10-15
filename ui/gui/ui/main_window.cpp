
#include "ui/gui/ui/main_window.hpp"

CMainWindow::CMainWindow() :
	QMainWindow(NULL)
{
	setupUi(this);
}

void CMainWindow::on_about_action_triggered(bool checked)
{
	CAboutDialog(this).exec();
}

