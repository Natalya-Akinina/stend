
#include "lib/image.hpp"

vector< shared_ptr<CImage> > images;

// ############################################################################

s_image * create_image(const unsigned height, const unsigned width, const unsigned ch_num)
{
	CImage * img = NULL;
	s_image * _img = NULL;

	try
	{
		throw_null(img = new CImage(height, width, ch_num));
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

int delete_image(const s_image * img)
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

CImage * CImage::to_CImage(s_image * img)
{
	return (CImage *) img->_img;
}

Mat * CImage::to_Mat(s_image * img)
{
	return (Mat *) img->mat;
}

s_image * CImage::from_Mat(const Mat & mat)
{
	images.push_back(shared_ptr<CImage>(new CImage(mat)));

	return & images[images.size() - 1]->img;
}

// ############################################################################

CImage::CImage(const unsigned height, const unsigned width, const unsigned ch_num)
{
	init(height, width, ch_num);
}

CImage::CImage(const Mat & mat)
{
	init(mat.rows, mat.cols, mat.channels());

	mat.copyTo(* (Mat *) img.mat);
	// TODO img.data
}

CImage::~CImage()
{
	if(img.mat != NULL)
		delete (Mat *) img.mat;

	// TODO img.data
}

void CImage::init(const unsigned height, const unsigned width, const unsigned ch_num)
{
	img.height = height;
	img.width = width;
	img.ch_num = ch_num;
	throw_null(img.mat = new Mat(height, width, CV_8UC(ch_num)));
	img.data = NULL; // TODO
	img._img = this;
}

