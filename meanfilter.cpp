/*
* @brief 均值滤波
* @author 吴驰域 3140100333
* @date 2017-05-03
*/

#include <windows.h>
#include "meanfilter.h"

unsigned char* meanfilter(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int w, int h)
{
	int m = bitmapInfoHeader->biHeight;		
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	unsigned char* newbitmapData = new unsigned char[m*n];
	int kernel_size = (2 * w + 1) * (2 * h + 1);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j+=3)		// 对(i, j)该点做卷积，累加后除以卷积核大小
		{
			int tmpr = 0, tmpg = 0, tmpb = 0;
			for (int k = i - h; k <= i + h; k++)
				for (int l = j - w*3; l <= j + w*3; l+=3)
				{
					if (k < 0 || l < 0 || k >= m || l >= n)
						continue;
					tmpb += bitmapData[k * n + l];		
					tmpg += bitmapData[k * n + l + 1];
					tmpr += bitmapData[k * n + l + 2];
				}
			newbitmapData[i * n + j] = tmpb / kernel_size;
			newbitmapData[i * n + j + 1] = tmpg / kernel_size;
			newbitmapData[i * n + j + 2] = tmpr / kernel_size;
		}

	return newbitmapData;
}