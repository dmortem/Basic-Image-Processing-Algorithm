/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Mirror.h"

#define BITMAP_ID 0x4D42

unsigned char* mirror(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int sx, int sy)
{
	unsigned char* newbitmapData = new unsigned char[bitmapInfoHeader->biSizeImage];

	int m = bitmapInfoHeader->biHeight;		
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;

	if (sx == 1)	// x轴做镜像变换
	{
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j += 3)
			{
				int x = m - i - 1, y = j;
				newbitmapData[i*n + j] = bitmapData[x*n + y];
				newbitmapData[i*n + j+1] = bitmapData[x*n + y+1];
				newbitmapData[i*n + j+2] = bitmapData[x*n + y+2];
			}
	}
	else {			// y轴做镜像变换
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j += 3)
			{
				int x = i, y = n - 3 - j;
				newbitmapData[i*n + j] = bitmapData[x*n + y];
				newbitmapData[i*n + j + 1] = bitmapData[x*n + y + 1];
				newbitmapData[i*n + j + 2] = bitmapData[x*n + y + 2];
			}
	}
	return newbitmapData;
}