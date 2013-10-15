
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "all.hpp"
#include "lib/interface.hpp"

class CImage
{
	void init(const unsigned height, const unsigned width, const unsigned ch_num);

	public:

		s_image img;

		CImage(const unsigned height, const unsigned width, const unsigned ch_num);
		CImage(const Mat & mat);
		~CImage();

		static void init();
		static void destroy();
		static void clear();
		static CImage * to_CImage(s_image * img);
		static Mat * to_Mat(s_image * img);
		static s_image * from_Mat(const Mat & mat);
};

#endif

