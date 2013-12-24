
#include "lib/opencv.hpp"

Mat get_struct_element (lua_State * state);
int Sobel_Scharr (lua_State * state, int type);

void opencv_register(lua_State * state)
{
	lua_register(state, "threshold", lua_threshold);
	lua_register(state, "split", lua_split);
	lua_register(state, "merge", lua_merge);
	lua_register(state, "bitwise_and", lua_bitwise_and);
	
	lua_register(state, "bitwise_not", lua_bitwise_not);
	lua_register(state, "bitwise_or", lua_bitwise_or);
	lua_register(state, "bitwise_xor", lua_bitwise_xor);
	lua_register(state, "adaptiveThreshold", lua_adaptiveThreshold);
	lua_register(state, "abs", lua_abs);
	lua_register(state, "dct", lua_dct);
	lua_register(state, "idct", lua_idct);
	lua_register(state, "dft", lua_dft);	
	lua_register(state, "idft", lua_idft);
	lua_register(state, "approxPolyDP", lua_approxPolyDP);
	lua_register(state, "arcLength", lua_arcLength);
	lua_register(state, "bilateralFilter", lua_bilateralFilter);
	
	lua_register(state, "image_create", lua_image_create);
	lua_register(state, "image_delete", lua_image_delete);
	lua_register(state, "matrix_create", lua_matrix_create);
	lua_register(state, "matrix_delete", lua_matrix_delete);
	
	lua_register(state, "matrix_get_value", lua_matrix_get_value);
	lua_register(state, "matrix_set_value", lua_matrix_set_value);
	lua_register(state, "image_get_value", lua_image_get_value);
	lua_register(state, "image_set_value", lua_image_set_value);
	
	lua_register(state, "erode", lua_erode);
	lua_register(state, "dilate",lua_dilate);
	lua_register(state, "morphologyEx", lua_morphologyEx);
	
	lua_register(state, "blur", lua_blur);
	lua_register(state, "cvtColor", lua_cvtColor);
	lua_register(state, "GaussianBlur", lua_GaussianBlur);
	lua_register(state, "medianBlur", lua_medianBlur);
	lua_register(state, "Canny", lua_Canny);
	lua_register(state, "Sobel", lua_Sobel);
	lua_register(state, "Scharr", lua_Scharr);
	lua_register(state, "filter2D", lua_filter2D);
	lua_register(state, "integral", lua_integral);
	lua_register(state, "Laplacian", lua_Laplacian);
	lua_register(state, "mulSpectrums", lua_mulSpectrums);
	lua_register(state, "sepFilter2D", lua_sepFilter2D);
	lua_register(state, "setTo", lua_setTo);
	lua_register(state, "clone", lua_clone);
	lua_register(state, "Size", lua_Size);
	lua_register(state, "imread", lua_imread);
	lua_register(state, "imwrite", lua_imwrite);
	
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

		thr = lua_tointeger(state, 2);
		type = lua_tointeger(state, 3);
		
		throw_if(thr > 255, "TODO");

		throw_null(dst_img = image_copy(* src), "TODO");
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

				break;
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

		throw_null(_ch_img = new image[size], "TODO");
		throw_null(_ch = new Mat *[size], "TODO");

		for(v = 0; v < size; v++)
		{
			throw_null(_ch_img[v] = image_copy(ch[v]), "TODO");
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

		throw_null(dst_img = image_create(ch_1->rows, ch_1->cols, 3), "TODO");
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

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
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

//Канальное побитовое НЕ
int lua_bitwise_not(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		bitwise_not(* op_1, * dst);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Поканальное побитовое ИЛИ
int lua_bitwise_or(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		Mat * op_2 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		bitwise_or(* op_1, * op_2, * dst);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Поканальное побитовое исключающее ИЛИ
int lua_bitwise_xor(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		Mat * op_2 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		bitwise_xor(* op_1, * op_2, * dst);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Адаптивное пороговое преобразование
int lua_adaptiveThreshold(lua_State * state)
{
  
	int ret = 1;
	image dst_img;
	Mat * dst; //вход и выход
    
	try 
	{
		unsigned thr, type, al, ok; //порог, обозначение вида преобразования, обозначение алгоритма преобразования, размер окресности преобразования
		Mat * src = CImage::to_Mat((s_image *) lua_touserdata(state, 1));

		thr = lua_tointeger(state, 2);
		al = lua_tointeger(state, 3);
		type = lua_tointeger(state, 4);
		ok = lua_tointeger(state, 5);
		
		throw_if(thr > 255, "TODO");

		throw_null(dst_img = image_copy(* src), "TODO");
		dst = CImage::to_Mat(dst_img);
		
		switch(al)
		{
			case 0:
			{
				switch(type)
				{
					case 0:
					{
						adaptiveThreshold(* src, * dst, thr, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_TOZERO_INV, ok, 1);
						lua_pushlightuserdata(state, dst);

						return ret;
					}
					case 1:
					{
						adaptiveThreshold(* src, * dst, thr, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, ok, 1);
						lua_pushlightuserdata(state, dst);

						return ret;
					}
				}
			}
			case 1:
			{
				switch(type)
				{
					case 0:
					{
						adaptiveThreshold(* src, * dst, thr, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_TOZERO_INV, ok, 1);
						lua_pushlightuserdata(state, dst);

						 return ret;
					}
					case 1:
					{
						adaptiveThreshold(* src, * dst, thr, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, ok, 1);
						lua_pushlightuserdata(state, dst);

						return ret;
					}
				}
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
    
//Вычисление абсолютного значения каждого элемента матрицы
int lua_abs(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;
	Mat * op_1;

	try
	{
		op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		*dst = abs(*op_1);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Дискретное косинусное преобразование
int lua_dct(lua_State * state)
{
  
	int ret = 1;
	image dst_img;
	Mat * dst;
	Mat * op_1;
	Mat op_gray,op_float, res;

	try
	{
		throw_null(op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1)), "TODO");
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		
		cvtColor (* op_1, op_gray, CV_RGB2GRAY);
		op_float = Mat_<float>(op_gray);
		dct (op_float, res, DCT_ROWS);
		* dst = Mat_<uint8_t>(res);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Обратное дискретное косинусное преобразование
int lua_idct(lua_State * state)
{
  
	int ret = 1;
	image dst_img;
	Mat * dst;
	Mat * op_1;
	Mat op_float, res;

	try
	{
		throw_null(op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1)), "TODO");
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		
		op_float = Mat_<float>(* op_1);
		dct (op_float, res, DCT_ROWS);
		* dst = Mat_<uint8_t>(res);
		
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);
		ret = 0;
	}

	return ret;
}

//Дискретное преобразование Фурье
int lua_dft(lua_State * state)
{
  
  
	int ret = 1;
	image dst_img;
	Mat * dst;
	Mat * op_1;
	Mat op_gray, I, dst1;
	vector <Mat> res;

	try
	{
		throw_null(op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1)), "TODO");
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		
		cvtColor (* op_1, op_gray, CV_RGB2GRAY);
		vector <Mat> ch_I = {Mat_<float>(op_gray), Mat::zeros(op_gray.size(), CV_32F)};
		merge(ch_I,I);
		

		dft(I, dst1);
		split (dst1, res);
		* dst = Mat_<uint8_t>(res[0]);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Обратное дискретное преобразование Фурье
int lua_idft(lua_State * state)
{
  
	int ret = 1;
	image dst_img;
	Mat * dst;
	Mat * op_1;
	Mat op_gray, I, dst1;
	vector <Mat> res;

	try
	{
		throw_null(op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1)), "TODO");
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		
		op_gray = * op_1;
		vector <Mat> ch_I = {Mat_<float>(op_gray), Mat::zeros(op_gray.size(), CV_32F)};
		merge(ch_I,I);

		idft (I, dst1);
		split (dst1, res);
		* dst = Mat_<uint8_t>(res[0]);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Аппроксимация ломаной линией с заданной точностью
int lua_approxPolyDP(lua_State * state)
{
  
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
		unsigned ep; //точность аппраксимации
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		
		ep = lua_tointeger(state, 2);
		
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		approxPolyDP(* op_1, * dst, ep, true);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Расчет периметра контура или длины кривой
int lua_arcLength (lua_State * state)
{
  
	int ret = 1;
	Mat * op_1;

	try
	{
		double res;
		throw_null(op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1)), "TODO");
		

		res = arcLength (* op_1, true);
		lua_pushnumber(state, res);
	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
}

//Двусторонняя фильтрация изображения
int lua_bilateralFilter (lua_State * state)
{
  
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
		unsigned d, sC, sS; //диаметр окресности каждого пикселя, значение фильтра в цветовом пространстве, значение фильтра в пространстве координат
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		
		d = lua_tointeger(state, 2);
		sC = lua_tointeger(state, 3);
		sS = lua_tointeger(state, 4);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		bilateralFilter(* op_1, * dst, d, sC, sS, BORDER_DEFAULT);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Создание изображения
int lua_image_create (lua_State * state)
{
  
	int ret = 1;
	image dst_img;

	try
	{
		unsigned h, w, ch;
		h = lua_tointeger(state, 1);
		w = lua_tointeger(state, 2);
		ch = lua_tointeger(state, 3);
		
		throw_null(dst_img = image_create(h, w, ch), "TODO");
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
  
}

//Удаление изображения
int lua_image_delete (lua_State * state)
{
	int ret = 0;
	image dst_img;
	
	try
	{	
		throw_null(dst_img = (s_image *) lua_touserdata(state, 1), "TODO");
		image_delete(dst_img);
	}
	catch(...)
	{;}
	
	return ret;
}

//Создание матрицы
int lua_matrix_create (lua_State * state)
{
  
	int ret = 1;
	matrix dst_mat;
	int type [3] = {UNSIGNED_INT_8_BIT_ELEMENT, UNSIGNED_INT_16_BIT_ELEMENT, DOUBLE_ELEMENT};

	try
	{
		unsigned h, w, ch, t;
		h = lua_tointeger(state, 1);
		w = lua_tointeger(state, 2);
		ch = lua_tointeger(state, 3);
		t = lua_tointeger(state, 4);
		
		
		throw_null(dst_mat = matrix_create(h, w, ch, type[t]), "TODO");
		lua_pushlightuserdata(state, dst_mat);
	}
	catch(...)
	{
		matrix_delete(dst_mat);

		ret = 0;
	}

	return ret;
  
}

//Удаление матрицы
int lua_matrix_delete (lua_State * state)
{
	int ret = 0;
	matrix dst_mat;
	
	try
	{
		throw_null(dst_mat = lua_touserdata(state, 1), "TODO");
		matrix_delete(dst_mat);
	}
	catch(...)
	{;}
	
	return ret;
}

//Получение значения элемента матрицы
int lua_matrix_get_value (lua_State * state)
{
  
	int ret = 1;
	matrix dst_mat;

	try
	{
		unsigned r, c, ch;
		int t;
		throw_null(dst_mat = lua_touserdata(state, 1), "TODO");
		r = lua_tointeger(state, 2);
		c = lua_tointeger(state, 3);
		ch = lua_tointeger(state, 4);
		
		
		matrix_element_type (dst_mat, & t);
		
		switch (t)
		{
			 case UNSIGNED_INT_8_BIT_ELEMENT:
			 {
				uint8_t res;
				matrix_get_value (dst_mat, r, c, ch, (void *) & res);
				lua_pushinteger (state, res);
				break;
			 }
			 case UNSIGNED_INT_16_BIT_ELEMENT:
			 {
				uint16_t res;
				matrix_get_value (dst_mat, r, c, ch, (void *) & res);
				lua_pushinteger (state, res);
				break;
			 }
			 case DOUBLE_ELEMENT:
			 {
				double res;
				matrix_get_value (dst_mat, r, c, ch, (void *) & res);
				lua_pushnumber (state, res);
				break;
			 }
		}
		
	}
	catch(...)
	{
	
		ret = 0;
	}

    return ret;
  
}

//Установ значения элемента матрицы
int lua_matrix_set_value (lua_State * state)
{
  
	int ret = 0;
	matrix dst_mat;

	try
	{
		unsigned r, c, ch;
		int t;
		throw_null(dst_mat = lua_touserdata(state, 1), "TODO");
		r = lua_tointeger(state, 2);
		c = lua_tointeger(state, 3);
		ch = lua_tointeger(state, 4);
		
		matrix_element_type (dst_mat, & t);
		
		switch (t)
		{
			 case UNSIGNED_INT_8_BIT_ELEMENT:
			 {
				uint8_t value;
				value = lua_tointeger(state, 5);
				matrix_set_value (dst_mat, r, c, ch, (void *) & value);
				break;
			 }
			 case UNSIGNED_INT_16_BIT_ELEMENT:
			 {
				uint16_t value;
				value = lua_tointeger(state, 5);
				matrix_set_value (dst_mat, r, c, ch, (void *) & value);
				break;
			 }
			 case DOUBLE_ELEMENT:
			 {
				double value;
				value = lua_tonumber(state, 5);
				matrix_set_value (dst_mat, r, c, ch, (void *) & value);
				break;
			 }
		}

	}
	catch(...)
	{
		matrix_delete(dst_mat);

		ret = 0;
	}

    return ret;
  
}

//Получение значения элемента изображения
int lua_image_get_value (lua_State * state)
{
  
	int ret = 1;
	Mat * op_1;

	try
	{
		unsigned r, c, ch;
		uint8_t res;
		throw_null(op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1)), "TODO");
		r = lua_tointeger(state, 2);
		c = lua_tointeger(state, 3);
		ch = lua_tointeger(state, 4);

		matrix_get_value (op_1, r, c, ch, (void *) & res);

		lua_pushinteger (state, res);
	}
	catch(...)
	{
		ret = 0;
	}

    return ret;
  
}

//Установ значения элемента изображения
int lua_image_set_value (lua_State * state)
{
  
	int ret = 1;
	Mat * op_1;

	try
	{
		unsigned r, c, ch;
		uint8_t value;
		throw_null(op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1)), "TODO");
		r = lua_tointeger(state, 2);
		c = lua_tointeger(state, 3);
		ch = lua_tointeger(state, 4);
		value = lua_tointeger(state, 5);
		
		matrix_set_value (op_1, r, c, ch, (void *) & value);

	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
  
}

//Размытие изображения
int lua_erode(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		    
		
		erode(* op_1, * dst, get_struct_element(state));
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Расширение изображения
int lua_dilate(lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		dilate(* op_1, * dst, get_struct_element(state));
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Расширенное морфологическое преобразование
int lua_morphologyEx(lua_State * state)
{
	int ret = 1;
	image dst_img;
	int type_morph;
	int morph [5] = {MORPH_OPEN, MORPH_CLOSE, MORPH_GRADIENT, MORPH_TOPHAT, MORPH_BLACKHAT};
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		type_morph = lua_tointeger(state, 2);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		morphologyEx(* op_1, * dst, morph[type_morph], get_struct_element(state));

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//
Mat get_struct_element (lua_State * state)
{
	int height, width;
	int type;
	int morph[3] = {MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS};
	int number;
	number = lua_gettop(state);
	
	number = number - 2;
	type = lua_tointeger(state, number);
	number ++;
	height = lua_tointeger(state, number);
	number ++;
	width = lua_tointeger(state, number);
	
	return getStructuringElement(morph[type], Size (width,height));
	
}

//Размытие изображения
int lua_blur (lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;
	int height, width;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		width = lua_tointeger(state, 2);
		height = lua_tointeger(state, 3);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		blur(* op_1, * dst, Size(width, height));
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
  
}

//Преобрахование изображения из одного цветового пространства в другое
int lua_cvtColor (lua_State * state)
{
	int ret = 1;
	image dst_img;
	int type;
	vector <int> type_color = {CV_RGB2GRAY, CV_GRAY2RGB,CV_BGR2XYZ, CV_RGB2XYZ, CV_XYZ2BGR, CV_XYZ2RGB, CV_BGR2YCrCb, CV_RGB2YCrCb, CV_YCrCb2BGR, CV_YCrCb2RGB, CV_BGR2HSV, CV_RGB2HSV, CV_HSV2BGR, CV_HSV2RGB, CV_BGR2HLS, CV_RGB2HLS, CV_HLS2BGR, CV_HLS2RGB, CV_BGR2Lab, CV_RGB2Lab, CV_Lab2BGR, CV_Lab2RGB, CV_BGR2Luv, CV_RGB2Luv, CV_Luv2BGR, CV_Luv2RGB};
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		type = lua_tointeger(state, 2);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		cvtColor(* op_1, * dst, type_color[type]);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Размытие изображения с помощью фильтра Гаусса
int lua_GaussianBlur (lua_State * state)
{
	int ret = 1;
	image dst_img;
	Mat * dst;
	int height, width;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		width = lua_tointeger(state, 2);
		height = lua_tointeger(state, 3);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);

		GaussianBlur(* op_1, * dst, Size(width, height), 0);
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
  
}

//Размытие изображения при помощи медианного фильтра
int lua_medianBlur (lua_State * state)
{
	int ret = 1;
	image dst_img;
	int type;
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		type = lua_tointeger(state, 2);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		medianBlur(* op_1, * dst, type);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Детектор границ
int lua_Canny (lua_State * state)
{
	int ret = 1;
	image dst_img;
	int size;
	double min, max;
	Mat * dst;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		min = lua_tonumber(state, 2);
		max = lua_tonumber(state, 3);
		size = lua_tointeger(state, 4);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		Canny(* op_1, * dst, min, max, size);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

// оператор Собеля
int lua_Sobel (lua_State * state)
{
	int size;
	size = lua_tointeger(state, 4);

	return Sobel_Scharr (state, size);
}

//оператор Щарра
int lua_Scharr (lua_State * state)
{
	return Sobel_Scharr (state, CV_SCHARR);
}

//
int Sobel_Scharr (lua_State * state, int type)
{
	int ret = 1;
	image dst_img;
	int xorder, yorder;
	Mat * dst;
	
	try
	{
		
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		xorder = lua_tointeger(state, 2);
		yorder = lua_tointeger(state, 3);
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		Sobel(* op_1, * dst, CV_8U, xorder, yorder, type);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Свертка изображения с ядром
int lua_filter2D (lua_State * state)
{
	int ret = 1;
	Mat * dst;
	image dst_img;
	
	try
	{
		
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		Mat * op_2 = CMatrix::to_Mat((matrix) lua_touserdata(state, 2));
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		filter2D (* op_1, * dst, CV_8U, * op_2);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Интегрирование изображения
int lua_integral (lua_State * state)
{
	int ret = 1;
	Mat * dst;
	image dst_img;
	
	try
	{
		
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		throw_null(dst_img = image_create((op_1->rows)+1, (op_1->cols)+1, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		integral(* op_1, * dst);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Лапласиан изображения
int lua_Laplacian (lua_State * state)
{
	int ret = 1;
	int size;
	Mat * dst;
	image dst_img;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		size = lua_tointeger(state, 2);

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		Laplacian(* op_1, * dst, size);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Поэлементное умножение двух спектров Фурье
int lua_mulSpectrums (lua_State * state)
{
	int ret = 1;
	Mat * dst;
	image dst_img;

	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		Mat * op_2 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));

		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		mulSpectrums(* op_1, * op_2, * dst, DFT_ROWS);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Линейная фильтрация
int lua_sepFilter2D (lua_State * state)
{
	int ret = 1;
	Mat * dst;
	image dst_img;
	
	try
	{
		
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		Mat * kernalX = CMatrix::to_Mat((matrix) lua_touserdata(state, 2));
		Mat * kernalY = CMatrix::to_Mat((matrix) lua_touserdata(state, 3));
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		sepFilter2D (* op_1, * dst, CV_8U, * kernalX, * kernalY);

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Устанавливает все элементы матрицы до указанного значения
int lua_setTo (lua_State * state)
{
	int ret = 0;
	int value;
	
	try
	{
		
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		value = lua_tointeger(state, 2);
		op_1->setTo(value);
	}
	catch(...)
	{
		;
	}

	return ret;
}

//Копирование изображения
int lua_clone (lua_State * state)
{
	int ret = 1;
	Mat * dst;
	image dst_img;
	
	try
	{
		
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		throw_null(dst_img = image_create(op_1->rows, op_1->cols, op_1->channels()), "TODO");
		dst = CImage::to_Mat(dst_img);
		* dst = op_1->clone();

		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);

		ret = 0;
	}

	return ret;
}

//Получение значений ширины, высоты и количества каналов
int lua_Size (lua_State * state)
{
	int ret = 3;
	int cols, rows, ch;
	
	try
	{
		
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 1));
		cols = op_1->cols;
		rows = op_1->rows;
		ch = op_1->channels();
		lua_pushinteger(state, rows);
		lua_pushinteger(state, cols);
		lua_pushinteger(state, ch);
		
	}
	catch(...)
	{
		ret = 0;
	}

	return ret;
}

//Чтение из памяти
int lua_imread (lua_State * state)
{
	int ret = 1;
	matrix dst = NULL;
	image dst_img = NULL;
	
	try
	{
		char * name = (char *) lua_tostring(state, 1);
		throw_null (dst = matrix_load_image (name), "TODO");
		throw_null (dst_img = matrix_to_image(dst), "TODO");
		
		lua_pushlightuserdata(state, dst_img);
	}
	catch(...)
	{
		image_delete(dst_img);
		matrix_delete(dst);

		ret = 0;
	}

	return ret;
	
}

//Запись по указанному адресу
int lua_imwrite (lua_State * state)
{
	int ret = 0;
	
	try
	{
		Mat * op_1 = CImage::to_Mat((s_image *) lua_touserdata(state, 2));
		char * name = (char *) lua_tostring(state, 1);
		
		imwrite (name, * op_1);
	}
	catch(...)
	{
		;
	}

	return ret;
}
