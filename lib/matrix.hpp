
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "all.hpp"
#include "lib/interface.h"

class CMatrix
{
	public:

		static void init();
		static void destroy();
		static void clear();

		static Mat * to_Mat(matrix mtx);
};

// Внутристендовые перегрузки - удобные, но возбуждающие исключения
unsigned my_matrix_height(matrix mtx);
unsigned my_matrix_width(matrix mtx);
unsigned my_matrix_number_of_channels(matrix mtx);
int my_matrix_element_type(matrix mtx);

#endif

