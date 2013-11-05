
#ifndef STAT_HPP
#define STAT_HPP

#include "all.hpp"
#include "config.hpp"

class CStat
{
	public:

		bool measure = false;

		CStat();
		virtual ~CStat();

		virtual void init() = 0;
		virtual void display() = 0;
		virtual void save(const QString fname) = 0;
};

class CSecPerFrameStat : public CStat
{
	vector<double> values;

	public:

		CSecPerFrameStat();
		~CSecPerFrameStat();

		void init();
		void operator()(const double value);
		void display();
		void save(const QString fname);
};

#endif

