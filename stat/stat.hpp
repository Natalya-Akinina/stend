
#ifndef STAT_HPP
#define STAT_HPP

#include "all.hpp"
#include "config.hpp"

class CSecPerFrameStat;

class CMeasure
{
	protected:

		QList<double> __sec_per_frame;

		friend class CSecPerFrameStat;

	public:

		CMeasure();
		~CMeasure();

		void reset();
		void operator()(const double sec_per_frame);
};

class CStat : public QObject
{
	Q_OBJECT

	protected:

		CMeasure & __measure; // TODO константа

	public:

		CStat(CMeasure & measure);
		virtual ~CStat();

		virtual QString name_en() const = 0;
		virtual QString name_ru() const = 0;

	public slots:

		virtual void display() = 0;
		virtual void save(const QString name = "") = 0;
};

class CSecPerFrameStat : public CStat
{
	Q_OBJECT

	public:

		CSecPerFrameStat(CMeasure & measure);
		~CSecPerFrameStat();

		QString name_en() const { return "sec_per_frame"; };
		QString name_ru() const { return trUtf8("Время выполнения алгоритма на кадре"); };

	public slots:

		void display();
		void save(const QString fname = "");
};

#endif

