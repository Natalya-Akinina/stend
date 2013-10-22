
#include "lib/opencv.hpp"

void opencv_register(lua_State * state)
{
	lua_register(state, "threshold", lua_threshold);
	lua_register(state, "split", lua_split);
	lua_register(state, "merge", lua_merge);
	lua_register(state, "bitwise_and", lua_bitwise_and);
}

// ############################################################################ 

// Пороговое преобразование
int lua_threshold(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst; //вход и выход

	try
	{
		unsigned thr, type; //порог и обозначение вида преобразования
		Mat * src = CImage::to_Mat((s_image *) lua_touserdata(state, 1));

		thr = lua_tonumber(state, 2);
		type = lua_tonumber(state, 3);
		
		throw_if(thr > 255);

		throw_null(dst_img = image_copy(* src));
		dst = CImage::to_Mat(dst_img);

		switch(type)
		{
			case 0:
			{
				threshold(* src, * dst, thr, 255, CV_THRESH_TOZERO_INV);
				lua_pushlightuserdata(state, dst_img);

				break;
			}
			case 1:
			{
				threshold(* src, * dst, thr, 255, CV_THRESH_BINARY);
				lua_pushlightuserdata(state, dst_img);

				return 1;
			}
		}
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

// Разделение на каналы
int lua_split(lua_State * state)
{
	image * _ch_img;
	Mat ** _ch;
	unsigned size, v;
	int ret;

	try
	{
		vector<Mat> ch;
	    Mat * src = CImage::to_Mat((s_image *) lua_touserdata(state, 1));

		split(* src, ch);
		size = ch.size();
		ret = size;

		throw_null(_ch_img = new image[size]);
		throw_null(_ch = new Mat *[size]);

		for(v = 0; v < size; v++)
		{
			throw_null(_ch_img[v] = image_copy(ch[v]));
			_ch[v] = CImage::to_Mat(_ch_img[v]);
		}

		for(v = 0; v < size; v++)
			lua_pushlightuserdata(state, _ch_img[v]);
	}
	catch(...)
	{
		if(_ch != NULL)
			for(v = 0; v < size; v++)
				image_delete(_ch_img[v]);

			delete [] _ch;

		ret = 0;
	}

	delete [] _ch_img;
	delete [] _ch;

    return ret;
}

// Объединение каналов
int lua_merge(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
	    vector<Mat> ch;
		Mat * ch_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
	    Mat * ch_2 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));
		Mat * ch_3 = CImage::to_Mat((s_image *) lua_touserdata(state, 3));

		throw_null(dst_img = image_create(ch_1->rows, ch_1->cols, 3));
		dst = CImage::to_Mat(dst_img);

		ch.push_back(* ch_1);
		ch.push_back(* ch_2);
		ch.push_back(* ch_3);

		merge(ch, * dst);

	    lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

    return ret;
}

// Поканальное побитовое И
int lua_bitwise_and(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
	    Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		Mat * op_2 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()));
		dst = CImage::to_Mat(dst_img);

		bitwise_and(* op_1, * op_2, * dst);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

    return ret;
}

