
#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/module.hpp"
#include "lib/matrix.hpp"
#include "lib/image.hpp"
#include "stat/stat.hpp"
#include "display/display.hpp"
#include "ui/main_window.hpp"

shared_ptr<CMainWindow> main_window;

int main(int argc, char * argv[])
{
	int ret;

	try
	{
		QApplication app(argc, argv);
		Q_INIT_RESOURCE(resources);

		// ############################################################################ 
		// Заставка

		QPixmap img(":/splash"); // TODO
		QSplashScreen splash(img);
		splash.show();

		app.processEvents();

		// ############################################################################ 
		// Инициализация стенда

		QString fname = QFileDialog::getOpenFileName(NULL, QObject::trUtf8("Загрузить конфигурационный файл"), "", QObject::trUtf8("Конфигурационный файл (*.xml)"));
		throw_if(fname.isEmpty(), "Конфигурационный файл загрузить не удалось");

		CConfig::load(fname);
		CMatrix::init();
		CImage::init();
		
		// ############################################################################ 

	//	QTest::qWait(2000); // TODO Раскомментировать

		CMainWindow * p_main_window;

		main_window.reset(new CMainWindow);
		throw_null(p_main_window = main_window.get(), "Не удалось создать главное окно программы");

		p_main_window->show();
		splash.finish(p_main_window);

		ret = app.exec();
	}
	catch(...)
	{
		ret = -1;
	}

	CMatrix::destroy();
	CImage::destroy();

	return ret;
}

void error_message(const char * msg)
{
	if(msg != NULL)
		QMessageBox::critical(NULL, QObject::trUtf8("Ошибка"), QObject::trUtf8(msg));
}

CDisplay * CDisplay::create(const unsigned fps)
{
	return new CQt5Display(main_window->src_video_window, main_window->dst_video_window, & main_window->is_experiment_run, 25);
}

