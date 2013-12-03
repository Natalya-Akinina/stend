
#ifndef MAIN_LOOP_HPP
#define MAIN_LOOP_HPP

#include "all.hpp"
#include "lib/lua.hpp"
#include "lib/image.hpp"
#include "main_loop/display.hpp"
#include "stat/stat.hpp"

class CMainLoop : public QObject
{
	Q_OBJECT

	bool __is_run = false;
	CLua & _lua;
	CMeasure measure;
	shared_ptr<CDisplay> disp;

	void start();

	public:

		typedef shared_ptr<CStat> PStat;
		QMap<QString, PStat> stats;

		CMainLoop(CLua & lua);
		~CMainLoop();

		inline bool is_run() { return __is_run; };
		void start(const QString src_fname, const QString dst_fname);
		void stop();

	signals:

		void __start();
		void __stop();
};

#endif

