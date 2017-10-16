/*
* @brief ֱ��ͼ���⻯
* @author ����� 3140100333
* @date 2017-04-12
*/

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "image_io.h"

using namespace std;

#define BITMAP_ID 0x4D42

unsigned char* equalization(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData)
{
	unsigned char* newbitmapData = new unsigned char[bitmapInfoHeader->biSizeImage];
	int m = bitmapInfoHeader->biHeight;		// **��������Ҫ��3
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	int *histogram_red = new int[256]();	// nk
	int *histogram_green = new int[256]();
	int *histogram_blue = new int[256]();
	double *sk_red = new double[256]();		// sk
	double *sk_green = new double[256]();
	double *sk_blue = new double[256]();
	int *skt_red = new int[256]();			// ~sk
	int *skt_green = new int[256]();
	int *skt_blue = new int[256]();
	int *num_red = new int[256]();			// new_nk
	int *num_green = new int[256]();
	int *num_blue = new int[256]();
	int size = bitmapInfoHeader->biWidth * bitmapInfoHeader->biHeight;
	// ��ֱ��ͼ
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			histogram_red[bitmapData[i*n + j + 2]] ++;
			histogram_green[bitmapData[i*n + j + 1]] ++;
			histogram_blue[bitmapData[i*n + j]] ++;
		}
	/* ����
	histogram_red[0] = 790;histogram_red[1] = 1023;histogram_red[2] = 850;
	histogram_red[3] = 656;histogram_red[4] = 329;histogram_red[5] = 245;
	histogram_red[6] = 122;histogram_red[7] = 81;size = 4096;
	*/
	// ���sk�ĸ���
	for (int i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			sk_red[i] += histogram_red[i];
			sk_green[i] += histogram_green[i];
			sk_blue[i] += histogram_blue[i];
		}
		else {
			sk_red[i] = sk_red[i - 1] + histogram_red[i];
			sk_green[i] = sk_green[i - 1] + histogram_green[i];
			sk_blue[i] = sk_blue[i - 1] + histogram_green[i];
			sk_red[i - 1] /= size;
			sk_green[i - 1] /= size;
			sk_blue[i - 1] /= size;
		}
	}
	sk_red[255] = sk_green[255] = sk_blue[255] = 1.0;
	// ����sk��ӽ�����skt(~s)�Լ���k��Ӧ��nkֵ�ۼӵ�skt�ϡ�skt�õ�������ӽ��������ţ�num�õ������·����nk
	int sum_red = 0, sum_green = 0, sum_blue = 0;
	for (int i = 0; i < 256; i++)
	{
		double min_red = 1.0, min_green = 1.0, min_blue = 1.0;
		// ��ɫ
		for (int j = sum_red; j < 256; j++)
		{
			double tmp = sk_red[i] - j *1.0 / 255;		// **��1.0
			if (tmp < 0)
				tmp = -tmp;
			if (min_red > tmp)
			{
				min_red = tmp;
				sum_red = j;
			}
			else break;
		}
		skt_red[i] = sum_red;
		num_red[sum_red] += histogram_red[i];
		// ��ɫ
		for (int j = sum_green; j < 256; j++)
		{
			double tmp = sk_green[i] - j *1.0 / 255;		// **��1.0
			if (tmp < 0)
				tmp = -tmp;
			if (min_green > tmp)
			{
				min_green = tmp;
				sum_green = j;
			}
			else break;
		}
		skt_green[i] = sum_green;
		num_green[sum_green] += histogram_green[i];
		// ��ɫ
		for (int j = sum_blue; j < 256; j++)
		{
			double tmp = sk_blue[i] - j *1.0 / 255;		// **��1.0
			if (tmp < 0)
				tmp = -tmp;
			if (min_blue > tmp)
			{
				min_blue = tmp;
				sum_blue = j;
			}
			else break;
		}
		skt_blue[i] = sum_blue;
		num_blue[sum_blue] += histogram_blue[i];
	}
	/* ������Խ��
	printf("k:	");
	for (int i = 0; i < 8; i++)
	{
	printf("%d	", i);
	}
	printf("\nnk:	");
	for (int i = 0; i < 8; i++)
	{
	printf("%d	", histogram_red[i]);
	}
	printf("\nsk:	");
	for (int i = 0; i < 8; i++)
	{
	printf("%.2f	", sk_red[i]);
	}
	printf("\n~sk:	");
	for (int i = 0; i < 8; i++)
	{
	printf("%d	", skt_red[i]);
	}
	printf("\nnew_nk:	");
	int pre = -1;
	for (int i = 0; i < 8; i++)
	{
	if (skt_red[i] != pre)
	printf("%d	", num_red[skt_red[i]]);
	else printf("	");
	pre = skt_red[i];
	}
	printf("\n");*/
	// �����������ص㣬�����µ�ֱ��ͼ�����µ�ֵ
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			newbitmapData[i*n + j + 2] = skt_red[bitmapData[i*n + j + 2]];
			newbitmapData[i*n + j + 1] = skt_green[bitmapData[i*n + j + 1]];
			newbitmapData[i*n + j] = skt_blue[bitmapData[i*n + j]];
		}
	return newbitmapData;
}

unsigned char* equalization_grey(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData)
{
	unsigned char* newbitmapData = new unsigned char[bitmapInfoHeader->biSizeImage];
	int m = bitmapInfoHeader->biHeight;		// **��������Ҫ��3
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	int *histogram = new int[256]();	// nk
	double *sk = new double[256]();		// sk
	int *skt = new int[256]();			// ~sk
	int *num = new int[256]();			// new_nk
	int size = bitmapInfoHeader->biWidth * bitmapInfoHeader->biHeight;
	// ��ֱ��ͼ
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			double y = 0.3 * bitmapData[i*n + j + 2] + 0.59 * bitmapData[i*n + j + 1] + 0.11 * bitmapData[i*n + j];
			histogram[(int)y] ++;
		}
	// ���sk�ĸ���
	for (int i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			sk[i] += histogram[i];
		}
		else {
			sk[i] = sk[i - 1] + histogram[i];
			sk[i - 1] /= size;
		}
	}
	sk[255] = 1.0;
	// ����sk��ӽ�����skt(~s)�Լ���k��Ӧ��nkֵ�ۼӵ�skt�ϡ�skt�õ�������ӽ���������(�Ҷ�ͼ����)������ԭ�����¼�������һ��ӳ�䣻num�õ������·����nk
	int sum = 0;
	for (int i = 0; i < 256; i++)
	{
		double min = 1.0;
		// ����ӽ���������䵥����ÿ�ο��Դ��ϴεĽ��sum��ʼö��
		for (int j = sum; j < 256; j++)
		{
			double tmp = sk[i] - j *1.0 / 255;		// **��1.0
			if (tmp < 0)
				tmp = -tmp;
			if (min > tmp)		// ��¼��С�ľ���ͱ��
			{
				min = tmp;
				sum = j;
			}
			else break;
		}
		skt[i] = sum;
		num[sum] += histogram[i];	// Ϊ�·ֵõĻҶȼ����ۻ����ص����
	}
	// �����������ص㣬�����µ�ֱ��ͼ�����µ�ֵ
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			// ����ԭ���ص�ĻҶȼ���ͨ��sktת�����µļ�����ת����RGB��
			double y = 0.3 * bitmapData[i*n + j + 2] + 0.59 * bitmapData[i*n + j + 1] + 0.11 * bitmapData[i*n + j];
			double u = 0.493 * (bitmapData[i*n + j] - y);
			double v = 0.877 * (bitmapData[i*n + j + 2] - y);
			double tmp = skt[(int)y];
			// ��ֹ������������������
			// ��ɫͨ��
			double color = u / 0.493 + tmp;
			if (color > 255)
				newbitmapData[i*n + j] = 255;
			else if (color < 0)
				newbitmapData[i*n + j] = 0;
			else newbitmapData[i*n + j] = color;
			// ��ɫͨ��
			color = v / 0.877 + tmp;
			if (color > 255)
				newbitmapData[i*n + j + 2] = 255;
			else if (color < 0)
				newbitmapData[i*n + j + 2] = 0;
			else newbitmapData[i*n + j + 2] = color;
			// ��ɫͨ��
			color = (tmp - 0.11 * bitmapData[i*n + j] - 0.3 * bitmapData[i*n + j + 2]) / 0.59;
			if (color > 255)
				newbitmapData[i*n + j + 1] = 255;
			else if (color < 0)
				newbitmapData[i*n + j + 1] = 0;
			else newbitmapData[i*n + j + 1] = color;
		}
	return newbitmapData;
}