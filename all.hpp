
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
// Исключения

void error_message(const char * msg);

#define throw_(msg) \
{\
	fprintf(stderr, "[Exception] File %s, line %d\n", __FILE__, __LINE__);\
	error_message(msg);\
	throw 1;\
};

#define throw_TODO(msg) \
{\
	fprintf(stderr, "TODO ");\
	throw_(msg);\
};

#define throw_if(condition, msg) \
{\
	if((condition))\
		throw_(msg);\
}

#define throw_null(pointer, msg) \
	throw_if((pointer) == NULL, msg)

// ############################################################################
// Отладочная печать

int printf_TODO(const char * format, ...);
int printf_error(const char * format, ...);

// ############################################################################ 
// Выделение памяти

void * alloc(const unsigned type_size, const unsigned dim, ...);

#endif

