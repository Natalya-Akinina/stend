
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "all.hpp"

class CDisplay
{
	protected:

		const unsigned __fps;

	public:

		CDisplay(const unsigned fps);
		virtual ~CDisplay(); // Без виртуального деструктора не будут вызваны деструкторы дочерних классов

		virtual void operator()(const Mat & src, const Mat & dst) = 0;
		virtual bool is_run() = 0;
		virtual void stop() = 0;

		static CDisplay * create(const unsigned fps);
};

class COpenCVDisplay : public CDisplay
{
	unsigned delay;
	const string src_wname = "Source", dst_wname = "Result";
	Mat __src, __dst;

	public:

		COpenCVDisplay(const unsigned fps);
		~COpenCVDisplay();

		void operator()(const Mat & src, const Mat & dst);
		bool is_run();
		void stop();
};

class CQt5Display : public CDisplay
{
	bool * __p_is_run;
	unsigned delay;
	QMdiSubWindow * __src_window, * __dst_window;

	public:

		CQt5Display(QMdiSubWindow * src_window, QMdiSubWindow * dst_window, bool * p_is_run, const unsigned fps);
		~CQt5Display();

		void operator()(const Mat & src, const Mat & dst);
		bool is_run();
		void stop();

		static void display(QMdiSubWindow * window, const Mat & img);
};


#endif

