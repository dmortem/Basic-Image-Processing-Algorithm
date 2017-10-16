/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scale.h"

#define BITMAP_ID 0x4D42

unsigned char* scale(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, double x, double y, int *h, int *w)
{
	int m = bitmapInfoHeader->biHeight;						// 原图大小，像素尺度
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	*h = m * x;												// 新图大小，坐标尺度
	*w = (int)(bitmapInfoHeader->biWidth * y + 3) / 4 * 4;
	int row = *h;											// 新图大小，像素尺度
	int col = *w * 3;
	unsigned char* newbitmapData = new unsigned char[row*col];

	for (int i = 0; i < row; i++)		// 枚举新图像素点，对应到原图中。最近邻插值
		for (int j = 0; j < col; j+=3)
		{
			int xx = i / x;
			int yy = (j/3) / y;		
			newbitmapData[i*col + j] = bitmapData[xx*n + yy*3];
			newbitmapData[i*col + j + 1] = bitmapData[xx*n + yy*3 + 1];
			newbitmapData[i*col + j + 2] = bitmapData[xx*n + yy*3 + 2];
		}
	return newbitmapData;
}