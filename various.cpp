
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

void * alloc(const unsigned type_size, const unsigned dim, ...)
{
	void * mem = NULL;
	va_list val;
	const unsigned dim_1 = dim - 1, dim_2 = dim - 2;
	unsigned u, v, num, size, step, * dim_size;
	char * pof, * dof, * t_dof; /* Стандартом гарантируется, что sizeof(char) == 1 */

	try
	{
		throw_null(dim_size = (unsigned *) alloca(dim * sizeof(unsigned)), "TODO");

		va_start(val, dim);

		for(u = 0, size = 0, num = 1; u < dim; u++)
		{
			num *= (dim_size[u] = va_arg(val, unsigned));
			size += (u == dim_1 ? type_size : sizeof(void *)) * num;
		}

		va_end(val);

		throw_null(mem = malloc(size), "TODO");

		for(u = 0, num = 1, dof = (char *) mem; u < dim_1; u++)
		{
			num *= dim_size[u];
			pof = dof;
			t_dof = (dof += num * sizeof(void *));
			step = (u == dim_2 ? type_size : sizeof(void *)) * dim_size[u + 1];

			for(v = 0; v < num; v++, pof += sizeof(void *), t_dof += step)
				memcpy(pof, & t_dof, sizeof(void *));
		}
	}
	catch(...)
	{
		free(mem);
		mem = NULL;
	}

	return mem;
}

