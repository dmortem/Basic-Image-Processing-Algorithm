/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
* @date 2017-04-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scale.h"

#define BITMAP_ID 0x4D42

unsigned char* scale(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, double x, double y, int *h, int *w)
{
	int m = bitmapInfoHeader->biHeight;						// ԭͼ��С�����س߶�
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	*h = m * x;												// ��ͼ��С������߶�
	*w = (int)(bitmapInfoHeader->biWidth * y + 3) / 4 * 4;
	int row = *h;											// ��ͼ��С�����س߶�
	int col = *w * 3;
	unsigned char* newbitmapData = new unsigned char[row*col];

	for (int i = 0; i < row; i++)		// ö����ͼ���ص㣬��Ӧ��ԭͼ�С�����ڲ�ֵ
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