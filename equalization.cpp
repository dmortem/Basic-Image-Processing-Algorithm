/*
* @brief 直方图均衡化
* @author 吴驰域 3140100333
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
	int m = bitmapInfoHeader->biHeight;		// **行数不需要乘3
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
	// 求直方图
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			histogram_red[bitmapData[i*n + j + 2]] ++;
			histogram_green[bitmapData[i*n + j + 1]] ++;
			histogram_blue[bitmapData[i*n + j]] ++;
		}
	/* 测试
	histogram_red[0] = 790;histogram_red[1] = 1023;histogram_red[2] = 850;
	histogram_red[3] = 656;histogram_red[4] = 329;histogram_red[5] = 245;
	histogram_red[6] = 122;histogram_red[7] = 81;size = 4096;
	*/
	// 求出sk的概率
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
	// 求与sk最接近的项skt(~s)以及把k对应的nk值累加到skt上。skt得到的是最接近的项的序号，num得到的是新分配的nk
	int sum_red = 0, sum_green = 0, sum_blue = 0;
	for (int i = 0; i < 256; i++)
	{
		double min_red = 1.0, min_green = 1.0, min_blue = 1.0;
		// 红色
		for (int j = sum_red; j < 256; j++)
		{
			double tmp = sk_red[i] - j *1.0 / 255;		// **乘1.0
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
		// 绿色
		for (int j = sum_green; j < 256; j++)
		{
			double tmp = sk_green[i] - j *1.0 / 255;		// **乘1.0
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
		// 蓝色
		for (int j = sum_blue; j < 256; j++)
		{
			double tmp = sk_blue[i] - j *1.0 / 255;		// **乘1.0
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
	/* 输出测试结果
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
	// 遍历所有像素点，根据新的直方图赋予新的值
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
	int m = bitmapInfoHeader->biHeight;		// **行数不需要乘3
	int n = (bitmapInfoHeader->biWidth + 3) / 4 * 4 * 3;
	int *histogram = new int[256]();	// nk
	double *sk = new double[256]();		// sk
	int *skt = new int[256]();			// ~sk
	int *num = new int[256]();			// new_nk
	int size = bitmapInfoHeader->biWidth * bitmapInfoHeader->biHeight;
	// 求直方图
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			double y = 0.3 * bitmapData[i*n + j + 2] + 0.59 * bitmapData[i*n + j + 1] + 0.11 * bitmapData[i*n + j];
			histogram[(int)y] ++;
		}
	// 求出sk的概率
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
	// 求与sk最接近的项skt(~s)以及把k对应的nk值累加到skt上。skt得到的是最接近的项的序号(灰度图级别)，即对原级别到新级别做了一个映射；num得到的是新分配的nk
	int sum = 0;
	for (int i = 0; i < 256; i++)
	{
		double min = 1.0;
		// 找最接近的项，利用其单调性每次可以从上次的结果sum开始枚举
		for (int j = sum; j < 256; j++)
		{
			double tmp = sk[i] - j *1.0 / 255;		// **乘1.0
			if (tmp < 0)
				tmp = -tmp;
			if (min > tmp)		// 记录最小的距离和标号
			{
				min = tmp;
				sum = j;
			}
			else break;
		}
		skt[i] = sum;
		num[sum] += histogram[i];	// 为新分得的灰度级别累积像素点个数
	}
	// 遍历所有像素点，根据新的直方图赋予新的值
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j += 3)
		{
			// 根据原像素点的灰度级别通过skt转换到新的级别，再转换回RGB。
			double y = 0.3 * bitmapData[i*n + j + 2] + 0.59 * bitmapData[i*n + j + 1] + 0.11 * bitmapData[i*n + j];
			double u = 0.493 * (bitmapData[i*n + j] - y);
			double v = 0.877 * (bitmapData[i*n + j + 2] - y);
			double tmp = skt[(int)y];
			// 防止溢出，考虑上溢和下溢
			// 蓝色通道
			double color = u / 0.493 + tmp;
			if (color > 255)
				newbitmapData[i*n + j] = 255;
			else if (color < 0)
				newbitmapData[i*n + j] = 0;
			else newbitmapData[i*n + j] = color;
			// 红色通道
			color = v / 0.877 + tmp;
			if (color > 255)
				newbitmapData[i*n + j + 2] = 255;
			else if (color < 0)
				newbitmapData[i*n + j + 2] = 0;
			else newbitmapData[i*n + j + 2] = color;
			// 绿色通道
			color = (tmp - 0.11 * bitmapData[i*n + j] - 0.3 * bitmapData[i*n + j + 2]) / 0.59;
			if (color > 255)
				newbitmapData[i*n + j + 1] = 255;
			else if (color < 0)
				newbitmapData[i*n + j + 1] = 0;
			else newbitmapData[i*n + j + 1] = color;
		}
	return newbitmapData;
}