
#include "all.hpp"
#include "ui/main_window.hpp"

int main(int argc, char * argv[])
{
	try
	{
		QApplication app(argc, argv);
		Q_INIT_RESOURCE(resources);

		CMainWindow main_window;
		main_window.show();

		return app.exec();
	}
	catch(...)
	{
		;
	}

	return -1;
}

