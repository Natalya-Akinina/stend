
#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/module.hpp"
#include "lib/matrix.hpp"
#include "lib/image.hpp"
#include "stat/stat.hpp"
#include "ui/main_window.hpp"

int main(int argc, char * argv[])
{
	int ret;

	try
	{
		QApplication app(argc, argv);
		qInstallMessageHandler(message_handler);
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

		CMainWindow main_window;

		main_window.show();
		splash.finish(& main_window);

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

void message(const QString msg)
{
	if(msg != NULL)
		QMessageBox::critical(NULL, QObject::trUtf8("Ошибка"), msg);
}

