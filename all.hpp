
#ifndef ALL_HPP
#define ALL_HPP

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <memory>
#include <unistd.h>
#include <map>
#include <time.h>
#include <QtCore>
#include <QApplication>
#include <QDialog>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QMenu>
#include <QToolButton>
#include <QSplashScreen>
#include <QtTest>
#include <QFileDialog>
#include <QSyntaxHighlighter>
#include <QListWidget>
#include <QTextBrowser>
#include <QMdiSubWindow>
#include <QLabel>
#include <lua.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// ############################################################################ 
// Вывод сообщений

void message(const QString msg);
void message_handler(QtMsgType type, const QMessageLogContext & context, const QString & msg);

// ############################################################################ 
// TODO

#define qTODO qDebug

// ############################################################################
// Исключения

#define throw_(msg) \
{\
	qCritical() << QObject::trUtf8(msg);\
	throw 1;\
};

#define throw_if(condition, msg) \
{\
	if((condition))\
		throw_(msg);\
}

#define throw_null(pointer, msg) \
	throw_if((pointer) == NULL, msg)

// ############################################################################ 
// Выделение памяти

void * alloc(const unsigned type_size, const unsigned dim, ...);

#endif

