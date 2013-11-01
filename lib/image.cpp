
#include "lib/image.hpp"

vector< shared_ptr<CImage> > images; 

// ############################################################################ 

image image_create(const unsigned height, const unsigned width, const unsigned ch_num)
{
	CImage * img = NULL;
	image _img = NULL;

	try
	{
		throw_null(img = new CImage(height, width, ch_num), "TODO");
		images.push_back(shared_ptr<CImage>(img));
		_img = & img->img;
	}
	catch(...)
	{
		delete img;
		_img = NULL;
	}

	return _img;
}

image image_copy(const Mat & img)
{
	throw_if(img.depth() != CV_8U, "TODO");
	
	image ret = image_create(img.rows, img.cols, img.channels());
	* (Mat *) ret->mat = img.clone();
	
	return ret;
}

int image_delete(const image img)
{
	const unsigned size = images.size();
	unsigned v;

	if(img != NULL)
	{
		for(v = 0; v < size; v++)
			if(& images[v]->img == img)
				images.erase(images.begin() + v);
	}

	return 0;
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

