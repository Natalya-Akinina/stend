
#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/module.hpp"
#include "lib/matrix.hpp"
#include "lib/image.hpp"
#include "stat/stat.hpp"

int main(const int argc, const char * argv[])
{
	int ret = 0;

	try
	{
		throw_if(argc != 5, "TODO");

		CConfig::load(argv[1]);
		CMatrix::init();
		CImage::init();

		CLua lua;
		CStat stat(lua);

		lua.load_module("/home/amv/disser/project/super_stend/src/modules/build/libdemo_image.so");
		// lua.load_module("demo_image");
		// lua.load_module("demo_matrix");

		lua.load_script(argv[2]);

		stat.run(argv[3], argv[4]);

		// stat.save_sec_per_frame("../../trash/sec_per_frame");
		stat.display_sec_per_frame();
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
		fprintf(stderr, "[Error message] %s\n", msg);
}

