
#include "lib/opencv.hpp"

void opencv_register(lua_State * state)
{
	lua_register(state, "delete", lua_delete);
	lua_register(state, "threshold", lua_threshold);
	lua_register(state, "split", lua_split);
	lua_register(state, "merge", lua_merge);
	lua_register(state, "bitwise_and", lua_bitwise_and);
}

// ############################################################################ 

// Удаление матрицы
int lua_delete(lua_State * state)
{
	// TODO
    Mat * img = (Mat *) lua_touserdata(state, 1);
	
	delete img;

	return 0;
}

// Пороговое преобразование
int lua_threshold(lua_State * state)
{
    Mat * src, dst; //вход и выход
	unsigned thr, type; //порог и обозначение вида преобразования

    src = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
	thr = lua_tonumber(state, 2);
    type = lua_tonumber(state, 3);
	
	dst = src->clone();

	if(thr > 255)
	{
		printf_error("lua_threshold: mp > 255\n");

        return 0;
    }

	switch(type)
	{
		case 0:
		{
			threshold(* src, dst, thr, 255, CV_THRESH_TOZERO_INV);
			lua_pushlightuserdata(state, CImage::from_Mat(dst));

			return 1;
		}
		case 1:
		{
			threshold(* src, dst, thr, 255, CV_THRESH_BINARY);
			lua_pushlightuserdata(state, CImage::from_Mat(dst));

			return 1;
		}
	}

	printf_error("lua_threshold: bz not in { 0, 1 }\n");

	return 0;
}

// Разделение на каналы
int lua_split(lua_State * state)
{
	Mat * src;
    vector<Mat> ch;

    src = CImage::to_Mat((s_image *) lua_touserdata(state, 1));

    split(* src, ch);

    lua_pushlightuserdata(state, CImage::from_Mat(ch[0]));
    lua_pushlightuserdata(state, CImage::from_Mat(ch[1]));
    lua_pushlightuserdata(state, CImage::from_Mat(ch[2]));

    return 3;
}

// Объединение каналов
int lua_merge(lua_State * state)
{
	Mat * ch_1, * ch_2, * ch_3, dst;
    vector<Mat> ch;

    ch_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
    ch_2 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));
    ch_3 = CImage::to_Mat((s_image *) lua_touserdata(state, 3));

	dst.create(ch_1->size(), CV_8UC3);

	ch.push_back(* ch_1);
	ch.push_back(* ch_2);
	ch.push_back(* ch_3);

    merge(ch, dst);

    lua_pushlightuserdata(state, CImage::from_Mat(dst));

    return 1;
}

// Поканальное побитовое И
int lua_bitwise_and(lua_State * state)
{
	Mat * op_1, * op_2, dst;

    op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
    op_2 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));

	dst.create(op_1->size(), CV_8U);

	bitwise_and(* op_1, * op_2, dst);

	lua_pushlightuserdata(state, CImage::from_Mat(dst));

    return 1;
}

