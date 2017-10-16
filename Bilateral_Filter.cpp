/*
* @brief ˫���˲�
* @author ����� 3140100333
* @date 2017-05-10
*/

#include "Bilateral_Filter.h"
#include "math.h"
#include <stdio.h>

#define M_PI	3.14159265358979323846

// һά��˹����
double Gaussian(double x, double sigma)
{
	return exp(-(x * x) / (2 * sigma * sigma)) / (sqrt(2 * M_PI) * sigma);
}

// ��ά��˹����
double Gaussian(double u, double v, double sigma)
{
	return exp(-(u * u + v * v) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
}

unsigned char* Bilateral_Filter(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int d, double sigma_color, double sigma_space)
{
	int m = bitmapInfoHeader->biHeight;
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	unsigned char* newbitmapData = new unsigned char[m*n];

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			// ��������Χ, xΪˮƽyΪ��ֱ
			int y_start = (i - d > 0) ? (i - d) : 0;
			int x_start = (j - 3 * d > 0) ? (j - 3 * d) : 0;
			int y_end = (i + d < m) ? (i + d) : m - 1;
			int x_end = (j + 3 * d < n) ? (j + 3 * d) : n - 1;

			/* ��ʼ����ʱ����, sum��¼������˹�����ĳ˻�����������һ����ans��¼��˹����������ֵ�ĳ˻���������Ŀ�꺯��
			   temp_space��temp_color�ֱ��¼������˹������ֵ */
			double sum0 = 0, sum1 = 0, sum2 = 0;
			double ans0 = 0, ans1 = 0, ans2 = 0;
			double temp_space = 0;
			double temp_color0 = 0, temp_color1 = 0, temp_color2 = 0;
			//printf("i=%d,j=%d\n", i, j);
			if (i == 634 && j == 2175)
			{
				int wcy = 1;
			}

			// ��ʼö�����������ص�
			for (int k = y_start; k <= y_end; k++)
				for (int l = x_start; l <= x_end; l += 3)
				{
					if (k == m && l == n)
						continue;
					// u, v��¼��ǰ���������Ŀռ���룻ic��¼��ɫ���
					int u = k - i, v = (l - j)/3;
					int ic0 = bitmapData[i*n + j] - bitmapData[k*n + l];
					int ic1 = bitmapData[i*n + j + 1] - bitmapData[k*n + l + 1];
					int ic2 = bitmapData[i*n + j + 2] - bitmapData[k*n + l + 2];

					temp_space = Gaussian(u, v, sigma_space);

					temp_color0 = Gaussian(ic0, sigma_color);
					temp_color1 = Gaussian(ic1, sigma_color);
					temp_color2 = Gaussian(ic2, sigma_color);

					sum0 += temp_space * temp_color0;
					sum1 += temp_space * temp_color1;
					sum2 += temp_space * temp_color2;

					ans0 += temp_space * temp_color0 * bitmapData[k*n + l];
					ans1 += temp_space * temp_color1 * bitmapData[k*n + l + 1];
					ans2 += temp_space * temp_color2 * bitmapData[k*n + l + 2];
				}
			newbitmapData[i*n + j] = ans0 / sum0;
			newbitmapData[i*n + j + 1] = ans1 / sum1;
			newbitmapData[i*n + j + 2] = ans2 / sum2;
		}
	return newbitmapData;
}
