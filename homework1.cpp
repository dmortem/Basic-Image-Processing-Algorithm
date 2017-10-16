/*
* @brief 图像读取、灰度化并输出保存为灰度图
* @author 吴驰域 3140100333
* @date 2017-03-27
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <windows.h> // 提供BITMAPINFOHEADER

using namespace std;

#define BITMAP_ID 0x4D42	// "MB"，表示这是bmp文件

// 读取bmp函数
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

// 输出灰度图。**可以考虑把YUV和RGB的互换新写一个函数
void output(char *filename, BITMAPINFOHEADER bitmapInfoHeader, unsigned char *bitmapData, double mul);

BITMAPFILEHEADER bitmapFileHeader;	// 读入图像bitmap文件头。**之后可以再考虑怎么写得好一点

int main(int argc, char *argv[])
{
	BITMAPINFOHEADER bitmapInfoHeader;	// bitmap信息头
	unsigned char *bitmapData;			// 位图数据
	unsigned char *new_bmpData;			// 新位图数据
	char filename[100];
	double mul;

	printf("Please input the name with .bmp of input image: ");
	scanf("%s", filename);
	printf("Please input the multiple of Y: ");
	scanf("%lf", &mul);

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);	// 读取图片数据

	output("gray.bmp", bitmapInfoHeader, bitmapData, mul);

	return 0;
}

// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;						// 文件指针
	unsigned char *bitmapImage;			// bitmap图像数据
	unsigned int imageIdx = 0;			// 图像位置索引
	unsigned char tempRGB;				// 交换变量

										// 以“二进制+读”模式打开文件filename。打开文件后即开始遍历该文件的文件头、信息头、调色板、数据 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
	{
		printf("Open file error!\n");
		printf("Please input like this: Monet.bmp");
		return NULL;
	}

	// 读入bitmap文件图
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader.bfType != BITMAP_ID)	// 根据文件头判断读入文件是否为bmp文件
	{
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);	// fread读完后filePtr文件指针自动移动，因此刚才读完文件头后，这里默认filePtr指向的就是信息头
																	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);			// SEEK_SET表示文件开头；SEEK_CUR表示当前位置；SEEK_END表示文件结尾（若是文件结尾，则第二个参数表示的是向前的OffSet位置）。这里fseek做的是从文件开头0处开始，偏移bfOffBits后的位置赋值给filePtr指针
																	// 为装载图像数据创建足够的内存
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// 验证内存是否创建成功
	if (!bitmapImage)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 读入bitmap图像数据
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// 确认读入成功
	if (bitmapImage == NULL)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// 关闭bitmap图像文件
	fclose(filePtr);
	return bitmapImage;
}

void output(char *filename, BITMAPINFOHEADER bitmapInfoHeader, unsigned char *bitmapData, double mul)
{
	unsigned char *gray_bmpData;		// 灰度图数据
	double *temp_Y, *temp_U, *temp_V;
	double max = 0;

// 把灰度图写入文件
	FILE *fp = fopen("gray.bmp", "wb");

// 定义文件头
	BITMAPFILEHEADER bfg;
	bfg.bfType = BITMAP_ID;
	bfg.bfReserved1 = 0;
	bfg.bfReserved2 = 0;
	bfg.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);	// 灰度图有调色版.OffBits指的是从文件头到数据开始的地方
																									// bfg.bfSize的值由OffBits+数据块大小算得

// 定义数据头
	BITMAPINFOHEADER big;
	memcpy(&big, &bitmapInfoHeader, sizeof(BITMAPINFOHEADER));		// 复制bitmaoInforHeader中的内容到big	
	// 随后只有biSizeImage和biBitCount需要修改。biWidth和biHeight指的都是一行/一列上的像素数，而灰度图和彩色图区别在于一个像素上的通道数，像素数是一致的
	int RowLength = ((bitmapInfoHeader.biWidth + 3) / 4) * 4;

	big.biSizeImage = RowLength * bitmapInfoHeader.biHeight;
	bfg.bfSize = bfg.bfOffBits + big.biSizeImage;
	big.biBitCount = 8;		// 1个像素8bit，能覆盖0-255；彩色图需要3*8=24

							// 信息头与数据头的输出
	fwrite(&bfg, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&big, sizeof(BITMAPINFOHEADER), 1, fp);

// 定义与输出调色板
	RGBQUAD *ipRGB2 = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
		ipRGB2[i].rgbRed = ipRGB2[i].rgbGreen = ipRGB2[i].rgbBlue = i;

	fwrite(ipRGB2, sizeof(RGBQUAD), 256, fp);

// 定义与输出数据
	int m = bitmapInfoHeader.biHeight, n = bitmapInfoHeader.biWidth;
	gray_bmpData = new unsigned char[big.biSizeImage];
	temp_Y = new double[big.biSizeImage];
	temp_U = new double[big.biSizeImage];
	temp_V = new double[big.biSizeImage];

	// RGB -> YUV，其中bitmapData存的顺序是BGR: bitmapData[3 * (i*RowLength + j)]是B；bitmapData[3 * (i*RowLength + j)+1]是G；bitmapData[3 * (i*RowLength + j)+2]是R
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
		{
			temp_Y[i*RowLength + j] = 0.11 * bitmapData[3 * (i*RowLength + j)] + 0.59 * bitmapData[3 * (i*RowLength + j) + 1] + 0.3 * bitmapData[3 * (i*RowLength + j) + 2];
			temp_U[i*RowLength + j] = 0.493 * (bitmapData[3 * (i*RowLength + j)] - temp_Y[i*RowLength + j]);
			temp_V[i*RowLength + j] = 0.877 * (bitmapData[3 * (i*RowLength + j) + 2] - temp_Y[i*RowLength + j]);
			if (temp_Y[i*RowLength + j] > max)
				max = temp_Y[i*RowLength + j];
		}

	// 归一化
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
			gray_bmpData[i*RowLength + j] = (unsigned char)(temp_Y[i*RowLength + j] / max * 255);

	fwrite(gray_bmpData, big.biSizeImage, 1, fp);
	fclose(fp);

// 转变为彩色图输出
	// 改变Y
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
			temp_Y[i*RowLength + j] *= mul;
	// YUV -> GRB
	unsigned char *new_bmpData = new unsigned char[bitmapInfoHeader.biSizeImage];			// 处理后的位图数据
	double temp_R, temp_G, temp_B;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
		{
			temp_B = temp_U[i*RowLength + j] / 0.493 + temp_Y[i*RowLength + j];
			temp_R = temp_V[i*RowLength + j] / 0.877 + temp_Y[i*RowLength + j];
			temp_G = (temp_Y[i*RowLength + j] - 0.3 * temp_R - 0.11 * temp_B)/0.59;
			// 由于Y变大了，转回来时要考虑溢出255
			if (temp_B > 255)
				new_bmpData[3 * (i*RowLength + j)] = 255;
			else new_bmpData[3 * (i*RowLength + j)] = (unsigned char)temp_B;
			if (temp_G > 255)
				new_bmpData[3 * (i*RowLength + j) + 1] = 255;
			else new_bmpData[3 * (i*RowLength + j) + 1] = (unsigned char)temp_G;
			if (temp_R > 255)
				new_bmpData[3 * (i*RowLength + j) + 2] = 255;
			else new_bmpData[3 * (i*RowLength + j) + 2] = (unsigned char)temp_R;
		}

// 输出新图像
	FILE *fp1 = fopen("result.bmp", "wb");
	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp1);
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp1);
	fwrite(new_bmpData, bitmapInfoHeader.biSizeImage, 1, fp1);
	fclose(fp1);

	return;
}