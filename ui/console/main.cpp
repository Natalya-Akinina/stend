
#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/module.hpp"
#include "lib/matrix.hpp"
#include "lib/image.hpp"
#include "main_loop/main_loop.hpp"
#include "main_loop/display.hpp"

int main(int argc, char * argv[])
{
	int ret = 0;

	try
	{
		qInstallMessageHandler(message_handler);
		Q_INIT_RESOURCE(scripts);

		throw_if(argc != 4, "Недостаточное количество аргументов");

		const QString script_fname = argv[1];
		const QString src_video_fname = argv[2];
		const QString dst_video_fname = argv[3];

		// ############################################################################ 

		CDisplay::init();
		CMatrix::init();
		CImage::init();

		CLua lua;
		CMainLoop main_loop(lua);

		lua.load_module("/home/amv/disser/project/super_stend/src/modules/build/demo_image/libdemo_image.so");

		lua.load_script(script_fname);
		main_loop.start(src_video_fname, dst_video_fname);

		main_loop.stats["sec_per_frame"]->display();
		main_loop.stats["sec_per_frame"]->save("/home/amv/trash/sec_per_frame");
	}
	catch(...)
	{
		ret = -1;
	}

	CDisplay::destroy();
	CMatrix::destroy();
	CImage::destroy();

	return ret;
}

void message(const QString msg)
{
	fprintf(stderr, "%s\n", msg.toLocal8Bit().constData());
}

