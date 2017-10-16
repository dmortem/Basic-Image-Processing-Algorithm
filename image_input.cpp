/*
* @brief bmp图像输入
* @author 吴驰域 3140100333
* @date 2017-04-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "image_io.h"

using namespace std;

#define BITMAP_ID 0x4D42

// 描述: 通过指针，返回filename 指定的bitmap文件中数据。
// 同时也返回bitmap信息头.（不支持-bit位图）
unsigned char *LoadBitmapFile(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader)
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
	fread(bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// 验证是否为bitmap文件
	if (bitmapFileHeader->bfType != BITMAP_ID)	// 根据文件头判断读入文件是否为bmp文件
	{
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// 读入bitmap信息头
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);	// fread读完后filePtr文件指针自动移动，因此刚才读完文件头后，这里默认filePtr指向的就是信息头
																	// 将文件指针移至bitmap数据
	fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);			// SEEK_SET表示文件开头；SEEK_CUR表示当前位置；SEEK_END表示文件结尾（若是文件结尾，则第二个参数表示的是向前的OffSet位置）。这里fseek做的是从文件开头0处开始，偏移bfOffBits后的位置赋值给filePtr指针
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