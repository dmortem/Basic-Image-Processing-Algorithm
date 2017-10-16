/*
* @brief 图像二值化与形态学变换
* @author 吴驰域 3140100333
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
	BITMAPINFOHEADER bitmapInfoHeader;		// bitmap信息头
	BITMAPFILEHEADER bitmapFileHeader;		// bitmap文件头
	unsigned char *bitmapData;				// 位图数据
	char filename[100];

	printf("Please input the name with .bmp of input image: ");
	scanf("%s", filename);

	bitmapData = LoadBitmapFile(filename, &bitmapFileHeader, &bitmapInfoHeader);	// 读取图片数据，返回信息头

	int m = bitmapInfoHeader.biHeight;		// **行数不需要乘3
	int n = (bitmapInfoHeader.biWidth + 3) / 4 * 4 * 3;
	// 自选阈值二值化
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
	// 输出二值图
	output("binary.bmp", &bitmapFileHeader, &bitmapInfoHeader, bitmapData);

	// 形态学变换，使用十字形的element structure
	const int dx[] = { 0, 0, -3, 0, 3, -6, -3, -3, 0, 0, 3, 3, 6 };		// dx方向上每移动一次要+-3。同时这里需要把自身也考虑进去
	const int dy[] = { 0, -1, 0, 1, 0, 0, 1, -1, 2, -2, 1, -1, 0 };
	int l = 13;

	//const int dx[] = { 0, 0, -3, 0, 3};		// dx方向上每移动一次要+-3。同时这里需要把自身也考虑进去
	//const int dy[] = { 0, -1, 0, 1, 0};
	//int l = 5;

	// 膨胀 Dilation
	unsigned char *data_dilation = new unsigned char[bitmapInfoHeader.biSizeImage];		// **必须重新定义，都在原数据上操作会导致全部变白。相当于是在更新后的图片上不断膨胀。重新定义保证了只更新一次
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
				if (tmp >= 0 && tmp < m*n && bitmapData[tmp] == 0)	// 邻近区域只要有一个为黑色，该点即为黑色
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)		// 邻域无黑色
				data_dilation[center] = data_dilation[center + 1] = data_dilation[center + 2] = 255;
			else data_dilation[center] = data_dilation[center + 1] = data_dilation[center + 2] = 0;		// **只改该点，不能改down,up,left,right四个点
		}
	output("dilation.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_dilation);
	
	// 腐蚀 Erosion
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
				if (tmp < 0 || tmp >= m*n || bitmapData[tmp] == 255)	// 邻近区域必须全为黑色，该点即为黑色。若有区域在图片外或是为白色，则该点即为白色
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)		// 邻域不是全黑
				data_erosion[center] = data_erosion[center + 1] = data_erosion[center + 2] = 255;
			else data_erosion[center] = data_erosion[center + 1] = data_erosion[center + 2] = 0;
		}
	output("erosion.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_erosion);

	// 开运算 Open
	unsigned char *data_open = new unsigned char[bitmapInfoHeader.biSizeImage];		// 腐蚀
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
				if (tmp < 0 || tmp >= m*n || bitmapData[tmp] == 255)	// 邻近区域必须全为黑色，该点即为黑色。若有区域在图片外或是为白色，则该点即为白色
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)		// 邻域不是全黑
				data_open[center] = data_open[center + 1] = data_open[center + 2] = 255;
			else data_open[center] = data_open[center + 1] = data_open[center + 2] = 0;
		}
	//output("open_1.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_open);
	unsigned char *data_open1 = new unsigned char[bitmapInfoHeader.biSizeImage];	// 膨胀
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
				if (tmp >= 0 && tmp < m*n && data_open[tmp] == 0)	// 邻近区域只要有一个为黑色，该点即为黑色
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)		// 邻域无黑色
				data_open1[center] = data_open1[center + 1] = data_open1[center + 2] = 255;
			else data_open1[center] = data_open1[center + 1] = data_open1[center + 2] = 0;		
		}
	output("open.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_open1);

	// 闭运算 close
	unsigned char *data_close = new unsigned char[bitmapInfoHeader.biSizeImage];	// 膨胀	
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
				if (tmp >= 0 && tmp < m*n && bitmapData[tmp] == 0)	// 邻近区域只要有一个为黑色，该点即为黑色
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)		// 邻域无黑色
				data_close[center] = data_close[center + 1] = data_close[center + 2] = 255;
			else data_close[center] = data_close[center + 1] = data_close[center + 2] = 0;		// **只改该点，不能改down,up,left,right四个点
		}
	//output("close_1.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_close);
	unsigned char *data_close1 = new unsigned char[bitmapInfoHeader.biSizeImage];		// 腐蚀
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
				if (tmp < 0 || tmp >= m*n || data_close[tmp] == 255)	// 邻近区域必须全为黑色，该点即为黑色。若有区域在图片外或是为白色，则该点即为白色
				{
					flag = 0;
					break;
				}
			}
			if (flag == 0)		// 邻域不是全黑
				data_close1[center] = data_close1[center + 1] = data_close1[center + 2] = 255;
			else data_close1[center] = data_close1[center + 1] = data_close1[center + 2] = 0;
		}
	output("close.bmp", &bitmapFileHeader, &bitmapInfoHeader, data_close1);

	return 0;
}
