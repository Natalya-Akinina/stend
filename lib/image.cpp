
#include "lib/image.hpp"

typedef QSharedPointer<CImage> PCImage;
QList<PCImage> images; 

// ############################################################################ 

image image_create(const unsigned height, const unsigned width, const unsigned ch_num)
{
	CImage * img = NULL;
	image _img = NULL;

	try
	{
		throw_null(img = new CImage(height, width, ch_num), "TODO");
		images.append(PCImage(img));
		_img = & img->img;
	}
	catch(...)
	{
		delete img;
		_img = NULL;
	}

	return _img;
}

int image_delete(const image img)
{
	const unsigned size = images.size();
	unsigned v;

	// TODO find или removeOne
	if(img != NULL)
	{
		for(v = 0; v < size; v++)
			if(& images[v]->img == img)
				images.removeAt(v);
	}

	return 0;
}

image image_copy(const Mat & img)
{
	throw_if(img.depth() != CV_8U, "TODO");
	
	image ret = image_create(img.rows, img.cols, img.channels());
	* (Mat *) ret->mat = img.clone();
	
	return ret;
}

image image_copy(const image img)
{
	return image_copy(* (Mat *) img->mat);
}

image matrix_to_image(const matrix mtx)
{
	image img;

	throw_null(img = image_create(my_matrix_height(mtx), my_matrix_width(mtx), my_matrix_number_of_channels(mtx)), "Не удалось создать описатель изображения");

	* ((Mat *) img->mat) = ((Mat *) mtx)->clone();

	return img;
}

// ############################################################################ 

CImage::CImage(const unsigned height, const unsigned width, const unsigned ch_num)
{
	img.height = height;
	img.width = width;
	img.ch_num = ch_num;
	throw_null(img.mat = matrix_create(height, width, ch_num, UNSIGNED_INT_8_BIT_ELEMENT), "TODO");
}

CImage::~CImage()
{
	matrix_delete(img.mat);
}

Mat * CImage::to_Mat(image img)
{
	return CMatrix::to_Mat(img->mat);
}

// ############################################################################ 

void CImage::init()
{
	;
}

void CImage::destroy()
{
	CImage::clear();
}

void CImage::clear()
{
	images.clear();
}

