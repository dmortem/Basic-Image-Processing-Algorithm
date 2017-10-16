/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Rotation.h"

#define BITMAP_ID 0x4D42
#define PI 3.1415926

unsigned char* rotation(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, double a, int *h, int *w)
{
	int m = bitmapInfoHeader->biHeight;			// 原图大小，像素级别
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	int xmin = m, ymin = n, xmax = 0, ymax = 0;	// 记录全图大小
	double angle = a * 1.0 / 360 * 2 * PI;		// math函数库中的参数是弧度制

	// 先求出新图像大小的尺寸
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int x = i*cos(angle) - (j/3)*sin(angle);	// 这里的x和y都是按照坐标计算的，因此必须把j/3换算成坐标值
			int y = i*sin(angle) + (j/3)*cos(angle);
  			if (x < xmin)
				xmin = x;
			if (x > xmax)
				xmax = x;
			if (y < ymin)
				ymin = y;
			if (y > ymax)
				ymax = y;
		}

	*h = xmax - xmin + 1;				// 新图大小，坐标级别，要加1
	*w = ymax - ymin + 1;
	int row = *h;						// 新图大小，像素级别
	int col = (*w + 3) / 4 * 4 * 3;
	unsigned char* newbitmapData = new unsigned char[row*col];

	// 初始化
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j += 3)
		{
			newbitmapData[i*col + j] = 255;
			newbitmapData[i*col + j + 1] = 255;
			newbitmapData[i*col + j + 2] = 255;
		}
	// 开始做变换
	for (int i = 0; i < m; i++)			// 这里是枚举原始图像坐标去映射到新图上
		for (int j = 0; j < n; j += 3)
		{
			int x = i*cos(angle) - (j / 3)*sin(angle)-xmin;		// 最小坐标为0，因此减去xmin转换到[0,w]的坐标空间。且在求对应坐标时，必须用(i,j/3)的坐标值进行计算
			int y = i*sin(angle) + (j / 3)*cos(angle)-ymin;
			newbitmapData[x*col + y * 3] = bitmapData[i*n + j];
			newbitmapData[x*col + y * 3 + 1] = bitmapData[i*n + j + 1];
			newbitmapData[x*col + y * 3 + 2] = bitmapData[i*n + j + 2];
		}
	// 插值
	for (int i = 0; i < row; i++)
		for (int j = col-3; j >= 0; j -= 3)
		{
			if (newbitmapData[i*col + j] == 255 && newbitmapData[i*col + j + 1] == 255 && newbitmapData[i*col + j + 2] == 255)
			{
				for (int k = j; k > 0 && k > j - 15; k -= 3)
					if (newbitmapData[i*col + k] != 255 || newbitmapData[i*col + k + 1] != 255 || newbitmapData[i*col + k + 2] != 255)
					{
						newbitmapData[i*col + j] = newbitmapData[i*col + k];
						newbitmapData[i*col + j + 1] = newbitmapData[i*col + k + 1];
						newbitmapData[i*col + j + 2] = newbitmapData[i*col + k + 2];
						break;
					}
			}
		}
	return newbitmapData;
}