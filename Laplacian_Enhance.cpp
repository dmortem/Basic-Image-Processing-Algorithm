/*
* @brief ������˹ͼ����ǿ
* @author ����� 3140100333
* @date 2017-05-03
*/

#include <windows.h>
#include "image_io.h"
#include "Laplacian_Enhance.h"

/* ������˹ͼ����ǿ, ѡ�õ�����ģ��Ϊ
	-1 -1 -1
	-1  8 -1
	-1 -1 -1
*/
unsigned char* Laplacian_Enhance(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData)
{
	int m = bitmapInfoHeader->biHeight;
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	unsigned char* newbitmapData = new unsigned char[m*n];
	int* f = new int[m*n];		// ������˹������õĶ��׵������п���Ϊ��������Ϊint
	unsigned char* ff = new unsigned char[m*n];
	int* kernel = new int[3 * 3]{ -1, -1, -1, -1, 8, -1, -1, -1, -1 };	// ���ﲻ�ܶ����unsigned char����Ϊ�и���

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int tmpb = 0, tmpg = 0, tmpr = 0;	// ��¼��ʱ��rgbֵ
			int sum = 0;						// ����ö�پ����
			if (bitmapData[i*n + j] > 250 && bitmapData[i*n + j + 1] > 250 && bitmapData[i*n + j + 2] > 250)
			{
				int wcy = 1;
			}
			for (int k = i - 1; k <= i + 1; k++)
				for (int l = j - 3; l <= j + 3; l+=3)
				{
					if (k < 0 || l < 0 || k >= m || l >= n)
						continue;
					tmpb += bitmapData[k*n + l] * kernel[sum];
					tmpg += bitmapData[k*n + l + 1] * kernel[sum];
					tmpr += bitmapData[k*n + l + 2] * kernel[sum];
					sum++;
				}
			f[i * n + j] = tmpb*10;		// ����Ķ��׵�����int���ͣ������и�
			f[i * n + j + 1] = tmpg*10;
			f[i * n + j + 2] = tmpr*10;

			// ����Ķ��׵���ֻ������
			if (tmpb > 255)
				ff[i * n + j] = 255;
			else if (tmpb < 0)
				ff[i * n + j] = 0;
			else ff[i * n + j] = tmpb;

			if (tmpg > 255)
				ff[i * n + j+1] = 255;
			else if (tmpg < 0)
				ff[i * n + j+1] = 0;
			else ff[i * n + j+1] = tmpg;

			if (tmpr > 255)
				ff[i * n + j+2] = 255;
			else if (tmpr < 0)
				ff[i * n + j+2] = 0;
			else ff[i * n + j+2] = tmpr;

			if (bitmapData[i * n + j] + ff[i * n + j] < 0)		// Խ���жϣ���Ϊ��ͼ����ǿ��һ��Ҫ��ֵǰ�ж�
				newbitmapData[i * n + j] = 0;
			else if (bitmapData[i * n + j] + ff[i * n + j] > 255)
				newbitmapData[i * n + j] = 255;
			else newbitmapData[i * n + j] = bitmapData[i * n + j] + ff[i * n + j];

			if (bitmapData[i * n + j + 1] + ff[i * n + j + 1] < 0)		
				newbitmapData[i * n + j + 1] = 0;
			else if (bitmapData[i * n + j + 1] + ff[i * n + j + 1] > 255)
				newbitmapData[i * n + j + 1] = 255;
			else newbitmapData[i * n + j + 1] = bitmapData[i * n + j + 1] + ff[i * n + j + 1];

			if (bitmapData[i * n + j + 2] + ff[i * n + j + 2] < 0)		
				newbitmapData[i * n + j + 2] = 0;
			else if (bitmapData[i * n + j + 2] + ff[i * n + j + 2] > 255)
				newbitmapData[i * n + j + 2] = 255;
			else newbitmapData[i * n + j + 2] = bitmapData[i * n + j + 2] + ff[i * n + j + 2];
		}
	/*int max = 0;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			if (ff[i*n + j] > max)
				max = ff[i*n + j];
		}
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			ff[i*n + j] = 255.0 / max * ff[i*n + j];
		}*/
	output("laptmp.bmp", bitmapFileHeader, bitmapInfoHeader, ff);
	return newbitmapData;
}