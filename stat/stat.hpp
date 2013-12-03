
#ifndef STAT_HPP
#define STAT_HPP

#include "all.hpp"
#include "main_loop/display.hpp"

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

		const CMeasure & __measure;

		virtual void save_data(QFile & fl) = 0;

	public:

		CStat(CMeasure & measure);
		virtual ~CStat();

		virtual QString name_en() const = 0;
		virtual QString name_ru() const = 0;

	public slots:

		virtual void display();
		void save(const QString fname = "");
};

class CSecPerFrameStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CSecPerFrameStat(CMeasure & measure);
		~CSecPerFrameStat();

		QString name_en() const { return "sec_per_frame"; };
		QString name_ru() const { return trUtf8("Время выполнения алгоритма на кадре"); };
};

#endif

