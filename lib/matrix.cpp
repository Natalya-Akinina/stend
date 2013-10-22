
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
			throw_;
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
				throw_null(mtx = new Mat(height, width, CV_8UC(ch_num)));

				break;
			};
			case DOUBLE_ELEMENT:
			{
				throw_null(mtx = new Mat(height, width, CV_64FC(ch_num)));

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
		throw_null(mtx);
		throw_null(_mtx = (Mat *) matrix_create(matrix_height(mtx), matrix_width(mtx), matrix_number_of_channel(mtx), matrix_element_type(mtx)));

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

		throw_if(img.empty());
		throw_null(mtx = (Mat *) matrix_create(img.rows, img.cols, img.channels(), depth_to_met(img.depth())));

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
	// TODO
	;
}

int matrix_get_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, void * value)
{
	// TODO
	;
}

int matrix_set_value(matrix mtx, const unsigned row, const unsigned column, const unsigned channel, const void * value)
{
	// TODO
	;
}

#define GET_INFO(fun, param)\
unsigned fun(matrix mtx)\
{\
	throw_null(mtx);\
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
		throw_null(value);\
		* value = fun(mtx);\
	}\
	catch(...)\
	{\
		ret = -1;\
	}\
\
	return ret;\
}

GET_INFO(matrix_height, rows)
GET_INFO(matrix_width, cols)
GET_INFO(matrix_number_of_channel, channels())

int matrix_element_type(matrix mtx)
{
	throw_null(mtx);

	switch(((Mat *) mtx)->depth())
	{
		case CV_8U:
			return UNSIGNED_INT_8_BIT_ELEMENT;
		case CV_64F:
			return DOUBLE_ELEMENT;
	}

	throw_;
}

int matrix_element_type(matrix mtx, int * value)
{
	int ret = 0;

	try
	{
		throw_null(value);
		* value = matrix_element_type(mtx);
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

