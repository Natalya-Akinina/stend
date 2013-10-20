
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "all.hpp"
#include "lib/interface.h"
#include "lib/matrix.hpp"

class CImage
{
	public:

		s_image img;

		CImage(const unsigned height, const unsigned width, const unsigned ch_num);
		~CImage();

		static void init();
		static void destroy();
		static void clear();

		static Mat * to_Mat(s_image * img);
};

// Удобная перегрузка
image image_copy(const Mat & img);

#endif

