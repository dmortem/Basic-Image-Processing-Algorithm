/*
* @brief ͼ�������
* @author ����� 3140100333
* @date 2017-04-12
*/

#include <windows.h>
#include <math.h>
#include "image_io.h"

using namespace std;

#define BITMAP_ID 0x4D42

unsigned char* logarithmic(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData)
{
	unsigned char* newbitmapData = new unsigned char[bitmapInfoHeader->biSizeImage];

	int m = bitmapInfoHeader->biHeight;		// **��������Ҫ��3
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	double Lmax = 0;
	// ת��ΪYUV��ɫ�ռ䣬��ͼ�������
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			double y = 0.3 * bitmapData[i*n + j + 2] + 0.59 * bitmapData[i*n + j + 1] + 0.11 * bitmapData[i*n + j];
			if (y > Lmax + 1e-6)
				Lmax = y;
		}
	// �ָ���RGB��ɫ�ռ�
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			// �������Ⱥ�ָ�RGB�����UVҲ�����
			double y = 0.3 * bitmapData[i*n + j + 2] + 0.59 * bitmapData[i*n + j + 1] + 0.11 * bitmapData[i*n + j];
			double u = 0.493 * (bitmapData[i*n + j] - y);
			double v = 0.877 * (bitmapData[i*n + j + 2] - y);
			double tmp = log(y + 1) / log(Lmax + 1) * 255;		// �������������0-1����һ����0-255
			// **ע���м�����������double���棬����ֱ����bitmapData����ͻ����
			newbitmapData[i*n + j] = u / 0.493 + tmp > 255 ? 255 : u / 0.493 + tmp;
			newbitmapData[i*n + j + 2] = v / 0.877 + tmp > 255 ? 255 : v / 0.877 + tmp;
			newbitmapData[i*n + j + 1] = (tmp - 0.11 * newbitmapData[i*n + j] - 0.3 * newbitmapData[i*n + j + 2]) / 0.59 > 255 ? 255 : (tmp - 0.11 * newbitmapData[i*n + j] - 0.3 * newbitmapData[i*n + j + 2]) / 0.59;
		}
	return newbitmapData;
}