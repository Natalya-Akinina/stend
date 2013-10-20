
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
#include <lua.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// ############################################################################
// Исключения

#define throw_ \
{\
	fprintf(stderr, "[Exception] File %s, line %d\n", __FILE__, __LINE__);\
	throw 1;\
};

#define throw_if(condition) \
{\
	if((condition))\
		throw_;\
}

#define throw_null(pointer) \
	throw_if((pointer) == NULL)

// ############################################################################
// Отладочная печать

int printf_TODO(const char * format, ...);
int printf_error(const char * format, ...);

// ############################################################################ 
// Выделение памяти

void * alloc(const unsigned type_size, const unsigned dim, ...);

#endif

