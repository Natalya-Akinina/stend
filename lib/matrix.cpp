
#include "lib/matrix.hpp"

vector< shared_ptr<Mat> > matrices;

int depth_to_met(const int depth)
{
	int type;

	switch(depth)
	{
		case CV_8U:
		{
			type = UNSIGNED_INT_8_BIT_ELEMENT;

			break;
		}
		case CV_64F:
		{
			type = DOUBLE_ELEMENT;

			break;
		}
		default:
		{
			throw_("TODO");
		}
	}

	return type;
}

// ############################################################################

matrix matrix_create(const unsigned height, const unsigned width, const unsigned ch_num, const int element_type)
{
	Mat * mtx = NULL;

	try
	{
		switch(element_type)
		{
			case UNSIGNED_INT_8_BIT_ELEMENT:
			{
				throw_null(mtx = new Mat(height, width, CV_8UC(ch_num)), "TODO");

				break;
			};
			case DOUBLE_ELEMENT:
			{
				throw_null(mtx = new Mat(height, width, CV_64FC(ch_num)), "TODO");

				break;
			};
		}

		matrices.push_back(shared_ptr<Mat>(mtx));
	}
	catch(...)
	{
		if(matrix_delete(mtx) && mtx != NULL)
			delete mtx;

		mtx = NULL;
	}

	return mtx;
}

matrix matrix_copy(matrix mtx)
{
	Mat * _mtx = NULL;
	
	try
	{
		throw_null(mtx, "TODO")
		throw_null(_mtx = (Mat *) matrix_create(my_matrix_height(mtx), my_matrix_width(mtx), my_matrix_number_of_channel(mtx), my_matrix_element_type(mtx)), "TODO");

		* _mtx = ((Mat *) mtx)->clone();
	}
	catch(...)
	{
		matrix_delete(_mtx);
	}

	return _mtx;
}

matrix matrix_load_image(const char * fname)
{
	Mat * mtx = NULL;
	
	try
	{
		Mat img = imread(fname);

		throw_if(img.empty(), "TODO");
		throw_null(mtx = (Mat *) matrix_create(img.rows, img.cols, img.channels(), depth_to_met(img.depth())), "TODO");

		* mtx = img.clone();
	}
	catch(...)
	{
		matrix_delete(mtx);
	}

	return mtx;
}

int matrix_save_image(matrix mtx, const char * fname)
{
	throw_TODO("TODO");
}

int matrix_get_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, void * value)
{
	int ret = 0;

	try
	{
		Mat * _mtx = (Mat *) mtx;

		switch(_mtx->depth())
		{

#define GET_VALUE(type_ind, type)\
case type_ind:\
{\
	* (type *) value = * ((type *) _mtx->data + _mtx->step[0] * row + _mtx->step[1] * column + channel);\
\
	break;\
}

			GET_VALUE(CV_8U, uint8_t)
			GET_VALUE(CV_64F, double)

			default:
			{
				throw_("TODO");
			}
		}
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

int matrix_set_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, const void * value)
{
	int ret = 0;

	try
	{
		Mat * _mtx = (Mat *) mtx;

		switch(_mtx->depth())
		{

#define SET_VALUE(type_ind, type)\
case type_ind:\
{\
	 * ((type *) _mtx->data + _mtx->step[0] * row + _mtx->step[1] * column + channel) = * (type *) value;\
\
	break;\
}

			SET_VALUE(CV_8U, uint8_t)
			SET_VALUE(CV_64F, double)

			default:
			{
				throw_("TODO");
			}
		}
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

#define GET_INFO(fun, my_fun, param)\
unsigned my_fun(matrix mtx)\
{\
	throw_null(mtx, "TODO");\
\
	return ((Mat *) mtx)->param;\
}\
\
int fun(matrix mtx, unsigned * value)\
{\
	int ret = 0;\
\
	try\
	{\
		throw_null(value, "TODO");\
		* value = my_fun(mtx);\
	}\
	catch(...)\
	{\
		ret = -1;\
	}\
\
	return ret;\
}

GET_INFO(matrix_height, my_matrix_height, rows)
GET_INFO(matrix_width, my_matrix_width, cols)
GET_INFO(matrix_number_of_channel, my_matrix_number_of_channel, channels())

int my_matrix_element_type(matrix mtx)
{
	throw_null(mtx, "TODO");

	switch(((Mat *) mtx)->depth())
	{
		case CV_8U:
			return UNSIGNED_INT_8_BIT_ELEMENT;
		case CV_64F:
			return DOUBLE_ELEMENT;
	}

	throw_("TODO");
}

int matrix_element_type(matrix mtx, int * value)
{
	int ret = 0;

	try
	{
		throw_null(value, "TODO");
		* value = my_matrix_element_type(mtx);
	}
	catch(...)
	{
		ret = -1;
	}
	
	return ret;
}

int matrix_delete(matrix mtx)
{
	if(mtx != NULL)
	{
		const unsigned size = matrices.size();
		unsigned v;

		for(v = 0; v < size; v++)
			if(matrices[v].get() == mtx)
			{
				matrices.erase(matrices.begin() + v);

				return 0;
			}
	}

	return -1;
}

int matrix_pointer_to_data(matrix mtx, void ** ptr)
{
	int ret = 0;

	try
	{
		throw_null(mtx, "TODO");

		* ptr = (void *) ((Mat *) mtx)->data;
	}
	catch(...)
	{
		ret = -1;
	}

	return ret;
}

// ############################################################################

void CMatrix::init()
{
	;
}

void CMatrix::destroy()
{
	CMatrix::clear();
}

void CMatrix::clear()
{
	matrices.clear();
}

Mat * CMatrix::to_Mat(matrix mtx)
{
	return (Mat *) mtx;
}

