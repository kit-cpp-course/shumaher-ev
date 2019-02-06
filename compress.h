#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;

class compressedBMP : public BMP
{
public:
	using BMP::BMP;

	/*
	Преобразование Добеши 4
	@param fname имя преобразуемого файла
	@param c порог сжатия
	*/
	void compress(const char *fname, float c)
	{
		size_t h = bmp_info_header.height;
		size_t w = bmp_info_header.width;
		
		for (int i = 0; i < 3; i++) 
		{
			compressed_data[i].resize(h * w);
		}

		for (size_t i = 0; i < h * w; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				compressed_data[k][i] = data[i * 3 + k];
			}
		}
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t i = 0; i < h; i++)
			{
				vector<float> v(w);
				for (size_t k = 0; k < w; k++)
				{
					v[k] = compressed_data[j][(i*w) + k];
				}
				vector<float> v2(compression(c, v));
				for (size_t k = 0; k < w; k++)
				{
					compressed_data[j][(i*w) + k] = v2[k];
				}
			}
		}
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t i = 0; i < w; i++)
			{
				vector<float> v(h);
				for (size_t k = 0; k < h; k++)
				{
					v[k] = compressed_data[j][(k*w) + i];
				}
				vector<float> v2(compression(c, v));
				for (size_t k = 0; k < h; k++)
				{
					compressed_data[j][(k*w) + i] = v2[k];
				}
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < h; j++)
			{
				vector<float> v(w);
				for (size_t k = 0; k < w; k++)
				{
					v[k] = compressed_data[i][(j*w) + k];
				}
				for (size_t k = 0; k < w; k += 2)
				{
					compressed_data[i][(j*w) + k / 2] = v[k];
					compressed_data[i][(j*w) + k / 2 + w / 2] = v[k + 1];
				}
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < w; j++)
			{
				vector<float> v(h);
				for (size_t k = 0; k < h; k++)
				{
					v[k] = compressed_data[i][(k*w) + j];
				}
				for (size_t k = 0; k < h; k += 2)
				{
					compressed_data[i][((k / 2)*w) + j] = v[k];
					compressed_data[i][((k / 2 + h / 2)*w)+ j] = v[k + 1];
				}
			}
		}
		for (size_t i = 0; i < h * w; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				data[i * 3 + k] = compressed_data[k][i]/(sqrt(2));
			}
		}
		write(fname);
	}

	/*
	Обратное преобразование Добеши 4
	@param fname имя преобразуемого файла
	@param c порог сжатия
	*/
	void decompress(const char *fname)
	{
		size_t h = bmp_info_header.height;
		size_t w = bmp_info_header.width;

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < w; j++)
			{
				vector<float> v(h);
				for (size_t k = 0; k < h; k++)
				{
					v[k] = compressed_data[i][(k * w) + j];
				}
				for (size_t k = 0; k < h; k += 2)
				{
					compressed_data[i][(k * w) + j] = v[k / 2];
					compressed_data[i][((k + 1) * w) + j] = v[k / 2 + h / 2];
				}
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < h; j++)
			{
				vector<float> v(w);
				for (size_t k = 0; k < w; k++)
				{
					v[k] = compressed_data[i][(j*w) + k];
				}
				for (size_t k = 0; k < w; k += 2)
				{
					compressed_data[i][(j*w) + k] = v[k / 2];
					compressed_data[i][(j*w) + k + 1] = v[k / 2 + w / 2];
				}
			}
		}
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t i = 0; i < w; i++)
			{
				vector<float> v(h);
				for (size_t k = 0; k < h; k++)
				{
					v[k] = compressed_data[j][(k*w) + i];
				}
				vector<float> v2(decompression(v));
				for (size_t k = 0; k < h; k++)
				{
					compressed_data[j][(k*w) + i] = v2[k];
				}
			}
		}
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t i = 0; i < h; i++)
			{
				vector<float> v(w);
				for (size_t k = 0; k < w; k++)
				{
					v[k] = compressed_data[j][(i*w) + k];
				}
				vector<float> v2(decompression(v));
				for (size_t k = 0; k < w; k++)
				{
					compressed_data[j][(i*w) + k] = v2[k];
				}
			}
		}
		for (size_t i = 0; i < h * w; i++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				data[i * 3 + k] = compressed_data[k][i];
			}
		}
		write(fname);
	}
protected:
	vector<float> compressed_data[3];
	
	/*
	Преобразование Добеши 4
	@param c порого сжатия
	@param v преобразуемй вектор
	@return преобразованный вектор
	*/
	vector<float> compression(float c, vector<float> v)
	{
		const size_t n = 4;
		float CL[n] = { (1 + sqrt(3)) / (4* sqrt(2)),
			(3 + sqrt(3)) / (4 * sqrt(2)),
				(3 - sqrt(3)) / (4 * sqrt(2)),
					(1 - sqrt(3)) / (4 * sqrt(2)) };
		float CH[n] = { (1 - sqrt(3)) / (4 * sqrt(2)),
			(sqrt(3) - 3) / (4 * sqrt(2)),
				(3 + sqrt(3)) / (4 * sqrt(2)),
					(-1 - sqrt(3)) / (4 * sqrt(2))};

		size_t len = v.size();
		vector<float> out(len);
		
		for (size_t k = 0; k < len; k += 2)
		{
			float sL = 0;
			float sH = 0;
			for (int i = 0; i < n; i++)
			{
				sL += v[(k + i) % len] * CL[i];
				sH += v[(k + i) % len] * CH[i];
			}
			out[k] = sL;
			if (sH < c) sH = 0;
			out[k + 1] = sH;
		}

		return out;
	}

	/*
	Обратное преобразование Добеши 4
	@param v преобразуемй вектор
	@return преобразованный вектор
	*/
	vector<float> decompression(vector<float> v)
	{
		const size_t n = 4;
		float CL[n] = { (3 - sqrt(3)) / (4 * sqrt(2)),
						(3 + sqrt(3)) / (4 * sqrt(2)), 
						(1 + sqrt(3)) / (4 * sqrt(2)),
						(1 - sqrt(3)) / (4 * sqrt(2))};
		float CH[n] = { (1 - sqrt(3)) / (4 * sqrt(2)),
						(-1 - sqrt(3)) / (4 * sqrt(2)),
						(3 + sqrt(3)) / (4 * sqrt(2)),
						(-3 + sqrt(3)) / (4 * sqrt(2))};

		size_t len = v.size();
		vector<float> out(len);

		for (size_t k = 0; k < len; k += 2)
		{
			float sL = 0;
			float sH = 0;
			for (int i = 0; i < n; i++)
			{
				sL += v[(k + i + len - 2) % len] * CL[i];
				sH += v[(k + i + len - 2) % len] * CH[i];
			}
			out[k] = sL;
			out[k + 1] = sH;
		}

		return out;
	}
};
