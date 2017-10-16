/*
* @brief 双边滤波
* @author 吴驰域 3140100333
* @date 2017-05-10
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "image_io.h"
#include "Bilateral_Filter.h"

using namespace std;

#define BITMAP_ID 0x4D42

int main(int argv, char* argc[])
{
	BITMAPINFOHEADER bitmapInfoHeader;		// bitmap信息头
	BITMAPFILEHEADER bitmapFileHeader;		// bitmap文件头
	unsigned char *bitmapData;				// 位图数据
	unsigned char *bifilterbitmapData;		// 双边滤波后数据
	double sigma_space, sigma_color;
	char filename[100];
	int d;

	printf("Please input the name with .bmp of input image: ");
	scanf("%s", filename);

	printf("Please input the size of the kernel in bilater filter: ");
	scanf("%d", &d);

	printf("Please input sigma_color and sigma_space: ");
	scanf("%lf%lf", &sigma_color, &sigma_space);

	bitmapData = LoadBitmapFile(filename, &bitmapFileHeader, &bitmapInfoHeader);	// 读取图片数据，返回信息头

																					// 均值滤波
	bifilterbitmapData = Bilateral_Filter(&bitmapFileHeader, &bitmapInfoHeader, bitmapData, d, sigma_color, sigma_space);
	output("bilateral_filter.bmp", &bitmapFileHeader, &bitmapInfoHeader, bifilterbitmapData);

	system("pause");
	return 0;
}