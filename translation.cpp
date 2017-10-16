/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "translation.h"

#define BITMAP_ID 0x4D42

unsigned char* translation(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, int y)
{
	int m = bitmapInfoHeader->biHeight + abs(x);		// 新图大小
	int n = (bitmapInfoHeader->biWidth + abs(y) + 3) / 4 * 4 * 3;
	int row = bitmapInfoHeader->biHeight;				// 原图大小
	int col = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	unsigned char* newbitmapData = new unsigned char[m*n];

	// i和j枚举新图像坐标
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int x1, y1;		// 原图像坐标
			if (x < 0)
				x1 = i;
			else x1 = i - x;
			if (y < 0)
				y1 = j;
			else y1 = j - 3 * y;
			if (x1 < 0 || y1 < 0 || x1 >= row || y1 >= col)		// 若新图像中的(i, j)未能对应到原图像，则赋为白色
			{
				newbitmapData[i * n + j] = 255;
				newbitmapData[i * n + j + 1] = 255;
				newbitmapData[i * n + j + 2] = 255;
			}
			else {
				newbitmapData[i * n + j] = bitmapData[x1 * col + y1];
				newbitmapData[i * n + j + 1] = bitmapData[x1 * col + y1 + 1];
				newbitmapData[i * n + j + 2] = bitmapData[x1 * col + y1 + 2];
			}
		}
	return newbitmapData;
}