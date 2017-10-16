/*
* @brief ͼ���ֵ������̬ѧ�任
* @author ����� 3140100333
* @date 2017-04-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//#include <opencv.hpp>
#include "image_io.h"

using namespace std;

#define BITMAP_ID 0x4D42

int main(int argv, char* argc[])
{
	BITMAPINFOHEADER bitmapInfoHeader;		// bitmap��Ϣͷ
	BITMAPFILEHEADER bitmapFileHeader;		// bitmap�ļ�ͷ
	unsigned char *bitmapData;				// λͼ����
	char filename[100];

	printf("Please input the name with .bmp of input image: ");
	scanf("%s", filename);

	bitmapData = LoadBitmapFile(filename, &bitmapFileHeader, &bitmapInfoHeader);	// ��ȡͼƬ���ݣ�������Ϣͷ

	int m = bitmapInfoHeader.biHeight;		// **��������Ҫ��3
	int n = (bitmapInfoHeader.biWidth + 3) / 4 * 4 * 3;
	// ��ѡ��ֵ��ֵ��
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			double y = 0.3 * bitmapData[i*n + j + 2] + 0.59 * bitmapData[i*n + j + 1] + 0.11 * bitmapData[i*n + j];
			if (y - 125 > 0.000001)
			{
				bitmapData[i*n + j] = bitmapData[i*n + j + 1] = bitmapData[i*n + j + 2] = 255;
			}
			else bitmapData[i*n + j] = bitmapData[i*n + j + 1] = bitmapData[i*n + j + 2] = 0;
		}
	// �����ֵͼ
	output("binary.bmp", &bitmapFileHeader, &bitmapInfoHeader, bitmapData);

	// ��̬ѧ�任��ʹ��ʮ���ε�element structure
	const int dx[] = { 0, 0, -3, 0, 3, -6, -3, -3, 0, 0, 3, 3, 6 };		// dx������ÿ�ƶ�һ��Ҫ+-3��ͬʱ������Ҫ������Ҳ���ǽ�ȥ
	const int dy[] = { 0, -1, 0, 1, 0, 0, 1, -1, 2, -2, 1, -1, 0 };
	int l = 13;

	//const int dx[] = { 0, 0, -3, 0, 3};		// dx������ÿ�ƶ�һ��Ҫ+-3��ͬʱ������Ҫ������Ҳ���ǽ�ȥ
	//const int dy[] = { 0, -1, 0, 1, 0};
	//int l = 5;

	// ���� Dilation
	unsigned char *data_dilation = new unsigned char[bitmapInfoHeader.biSizeImage];		// **�������¶��壬����ԭ�����ϲ����ᵼ��ȫ����ס��൱�����ڸ��º��ͼƬ�ϲ������͡����¶��屣֤��ֻ����һ��
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int flag = 1;
			int center = i*n + j;
			for (int k = 0; k < l; k++)
			{
				int ii = i + dy[k];
				int jj = j + dx[k];
				int tmp = ii*n + jj;
				if (tmp >= 0 && tmp < m*n && bitmapData[tmp] == 0)	// �ڽ�����ֻҪ��һ��Ϊ��ɫ���õ㼴Ϊ��ɫ
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)		// �����޺�ɫ
				data_dilation[center] = data_dilation[center + 1] = data_dilation[center + 2] = 255;
			else data_dilation[center] = data_dilation[center + 1] = data_dilation[center + 2] = 0;		// **ֻ�ĸõ㣬���ܸ�down,up,left,right�ĸ���
		}
	output("dilation.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_dilation);
	
	// ��ʴ Erosion
	unsigned char *data_erosion = new unsigned char[bitmapInfoHeader.biSizeImage];
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int flag = 1;
			int center = i*n + j;
			for (int k = 0; k < l; k++)
			{
				int ii = i + dy[k];
				int jj = j + dx[k];
				int tmp = ii*n + jj;
				if (tmp < 0 || tmp >= m*n || bitmapData[tmp] == 255)	// �ڽ��������ȫΪ��ɫ���õ㼴Ϊ��ɫ������������ͼƬ�����Ϊ��ɫ����õ㼴Ϊ��ɫ
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)		// ������ȫ��
				data_erosion[center] = data_erosion[center + 1] = data_erosion[center + 2] = 255;
			else data_erosion[center] = data_erosion[center + 1] = data_erosion[center + 2] = 0;
		}
	output("erosion.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_erosion);

	// ������ Open
	unsigned char *data_open = new unsigned char[bitmapInfoHeader.biSizeImage];		// ��ʴ
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int flag = 1;
			int center = i*n + j;
			for (int k = 0; k < l; k++)
			{
				int ii = i + dy[k];
				int jj = j + dx[k];
				int tmp = ii*n + jj;
				if (tmp < 0 || tmp >= m*n || bitmapData[tmp] == 255)	// �ڽ��������ȫΪ��ɫ���õ㼴Ϊ��ɫ������������ͼƬ�����Ϊ��ɫ����õ㼴Ϊ��ɫ
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)		// ������ȫ��
				data_open[center] = data_open[center + 1] = data_open[center + 2] = 255;
			else data_open[center] = data_open[center + 1] = data_open[center + 2] = 0;
		}
	//output("open_1.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_open);
	unsigned char *data_open1 = new unsigned char[bitmapInfoHeader.biSizeImage];	// ����
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int flag = 1;
			int center = i*n + j;
			for (int k = 0; k < l; k++)
			{
				int ii = i + dy[k];
				int jj = j + dx[k];
				int tmp = ii*n + jj;
				if (tmp >= 0 && tmp < m*n && data_open[tmp] == 0)	// �ڽ�����ֻҪ��һ��Ϊ��ɫ���õ㼴Ϊ��ɫ
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)		// �����޺�ɫ
				data_open1[center] = data_open1[center + 1] = data_open1[center + 2] = 255;
			else data_open1[center] = data_open1[center + 1] = data_open1[center + 2] = 0;		
		}
	output("open.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_open1);

	// ������ close
	unsigned char *data_close = new unsigned char[bitmapInfoHeader.biSizeImage];	// ����	
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int flag = 1;
			int center = i*n + j;
			for (int k = 0; k < l; k++)
			{
				int ii = i + dy[k];
				int jj = j + dx[k];
				int tmp = ii*n + jj;
				if (tmp >= 0 && tmp < m*n && bitmapData[tmp] == 0)	// �ڽ�����ֻҪ��һ��Ϊ��ɫ���õ㼴Ϊ��ɫ
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)		// �����޺�ɫ
				data_close[center] = data_close[center + 1] = data_close[center + 2] = 255;
			else data_close[center] = data_close[center + 1] = data_close[center + 2] = 0;		// **ֻ�ĸõ㣬���ܸ�down,up,left,right�ĸ���
		}
	//output("close_1.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_close);
	unsigned char *data_close1 = new unsigned char[bitmapInfoHeader.biSizeImage];		// ��ʴ
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			int flag = 1;
			int center = i*n + j;
			for (int k = 0; k < l; k++)
			{
				int ii = i + dy[k];
				int jj = j + dx[k];
				int tmp = ii*n + jj;
				if (tmp < 0 || tmp >= m*n || data_close[tmp] == 255)	// �ڽ��������ȫΪ��ɫ���õ㼴Ϊ��ɫ������������ͼƬ�����Ϊ��ɫ����õ㼴Ϊ��ɫ
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)		// ������ȫ��
				data_close1[center] = data_close1[center + 1] = data_close1[center + 2] = 255;
			else data_close1[center] = data_close1[center + 1] = data_close1[center + 2] = 0;
		}
	output("close.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_close1);

	return 0;
}
