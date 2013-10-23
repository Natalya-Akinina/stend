
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

matrix matrix_create(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type);
matrix matrix_copy(matrix mtx);
matrix matrix_load_image(const char * fname);
int matrix_save_image(matrix mtx, const char * fname);
int matrix_get_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, void * value);
int matrix_set_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, const void * value);
int matrix_height(matrix mtx, unsigned * value);
int matrix_width(matrix mtx, unsigned * value);
int matrix_number_of_channel(matrix mtx, unsigned * value);
int matrix_element_type(matrix mtx, int * value);
int matrix_delete(matrix mtx);
int matrix_pointer_to_data(matrix mtx, void ** ptr);

// Внутристендовые перегрузки - удобные, но возбуждающие исключения
unsigned matrix_height(matrix mtx);
unsigned matrix_width(matrix mtx);
unsigned matrix_number_of_channel(matrix mtx);
int matrix_element_type(matrix mtx);

#endif

