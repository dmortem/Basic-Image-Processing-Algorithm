/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
* @date 2017-04-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Mirror.h"

#define BITMAP_ID 0x4D42

unsigned char* shear(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, double d, int *h, int *w)
{
	int m = bitmapInfoHeader->biHeight;						// ԭͼ��С�����س߶�
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;

	if (x == 1)		// ��x��������
	{
		*h = m + abs((int)(d * bitmapInfoHeader->biWidth));		// ��ͼ��С������߶�
		*w = bitmapInfoHeader->biWidth;
		int row = *h;											// ��ͼ��С�����س߶�
		int col = *w * 3;
		unsigned char* newbitmapData = new unsigned char[row*col];

		for (int i = 0; i < row; i++)			// ö����ͼ
			for (int j = 0; j < col; j += 3)
			{
				int xx = i - d * (j / 3);
				int yy = j / 3;
				if (xx < 0 || xx > m)
				{
					newbitmapData[i*col + j] = 255;
					newbitmapData[i*col + j + 1] = 255;
					newbitmapData[i*col + j + 2] = 255;
				}
				else {
					newbitmapData[i*col + j] = bitmapData[xx*n + yy * 3];
					newbitmapData[i*col + j + 1] = bitmapData[xx*n + yy * 3 + 1];
					newbitmapData[i*col + j + 2] = bitmapData[xx*n + yy * 3 + 2];
				}
			}
		return newbitmapData;
	}
	else {		// ��y��������
		*h = m;													// ��ͼ��С������߶�
		*w = (bitmapInfoHeader->biWidth + abs((int)(d * bitmapInfoHeader->biHeight))+3)/4*4;	// ������4�ı���
		int row = *h;											// ��ͼ��С�����س߶�
		int col = *w * 3;
		unsigned char* newbitmapData = new unsigned char[row*col];

		for (int i = 0; i < row; i++)			// ö����ͼ
			for (int j = 0; j < col; j += 3)
			{
				if (i > 500 && j > 3000)
				{
					int p = 1;
				}
				int xx = i;
				int yy = j / 3 - d * i;
				if (yy < 0 || yy > bitmapInfoHeader->biWidth)
				{
					newbitmapData[i*col + j] = 255;
					newbitmapData[i*col + j + 1] = 255;
					newbitmapData[i*col + j + 2] = 255;
				}
				else {
					newbitmapData[i*col + j] = bitmapData[xx*n + yy * 3];
					newbitmapData[i*col + j + 1] = bitmapData[xx*n + yy * 3 + 1];
					newbitmapData[i*col + j + 2] = bitmapData[xx*n + yy * 3 + 2];
				}
			}
		return newbitmapData;
	}
}