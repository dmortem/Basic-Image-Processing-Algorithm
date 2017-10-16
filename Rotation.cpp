/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
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
	int m = bitmapInfoHeader->biHeight;			// ԭͼ��С�����ؼ���
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	int xmin = m, ymin = n, xmax = 0, ymax = 0;	// ��¼ȫͼ��С
	double angle = a * 1.0 / 360 * 2 * PI;		// math�������еĲ����ǻ�����

	// �������ͼ���С�ĳߴ�
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int x = i*cos(angle) - (j/3)*sin(angle);	// �����x��y���ǰ����������ģ���˱����j/3���������ֵ
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

	*h = xmax - xmin + 1;				// ��ͼ��С�����꼶��Ҫ��1
	*w = ymax - ymin + 1;
	int row = *h;						// ��ͼ��С�����ؼ���
	int col = (*w + 3) / 4 * 4 * 3;
	unsigned char* newbitmapData = new unsigned char[row*col];

	// ��ʼ��
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j += 3)
		{
			newbitmapData[i*col + j] = 255;
			newbitmapData[i*col + j + 1] = 255;
			newbitmapData[i*col + j + 2] = 255;
		}
	// ��ʼ���任
	for (int i = 0; i < m; i++)			// ������ö��ԭʼͼ������ȥӳ�䵽��ͼ��
		for (int j = 0; j < n; j += 3)
		{
			int x = i*cos(angle) - (j / 3)*sin(angle)-xmin;		// ��С����Ϊ0����˼�ȥxminת����[0,w]������ռ䡣�������Ӧ����ʱ��������(i,j/3)������ֵ���м���
			int y = i*sin(angle) + (j / 3)*cos(angle)-ymin;
			newbitmapData[x*col + y * 3] = bitmapData[i*n + j];
			newbitmapData[x*col + y * 3 + 1] = bitmapData[i*n + j + 1];
			newbitmapData[x*col + y * 3 + 2] = bitmapData[i*n + j + 2];
		}
	// ��ֵ
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