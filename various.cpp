
#include "all.hpp"

int printf_TODO(const char * format, ...)
{
	int ret = 0;
	char buf[1024 + strlen(format)];
	va_list args;

	sprintf(buf, "[ TODO ] %s", format);

	va_start(args, format);
	ret = vprintf(buf, args);
	va_end(args);

	return ret;
}

int printf_error(const char * format, ...)
{
	int ret = 0;
	char buf[1024 + strlen(format)];
	va_list args;

	sprintf(buf, "[ Error ] %s", format);

	va_start(args, format);
	ret = vfprintf(stderr, buf, args);
	va_end(args);

	return ret;
}
