/*
* @brief bmp图像输出
* @author 吴驰域 3140100333
* @date 2017-04-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "image_io.h"

using namespace std;

#define BITMAP_ID 0x4D42

void output(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, int y)
{
	FILE *fp = fopen(filename, "wb");

	BITMAPFILEHEADER bfg;
	bfg.bfType = BITMAP_ID;
	bfg.bfReserved1 = 0;
	bfg.bfReserved2 = 0;
	bfg.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER big;
	memcpy(&big, bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
	int RowLength = ((y + 3) / 4) * 4 * 3;
	int ColLength = x;
	big.biWidth = (y + 3) / 4 * 4;		// biWidth必须是4的倍数！！而不是一整行的字节数
	big.biHeight = ColLength;
	big.biSizeImage = RowLength * ColLength;
	bfg.bfSize = bfg.bfOffBits + big.biSizeImage;
	big.biBitCount = 24;

	fwrite(&bfg, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&big, sizeof(BITMAPINFOHEADER), 1, fp);

	fwrite(bitmapData, big.biSizeImage, 1, fp);
	fclose(fp);
}

void output(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData)
{
	FILE *fp = fopen(filename, "wb");
	fwrite(bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	fwrite(bitmapData, bitmapInfoHeader->biSizeImage, 1, fp);
	fclose(fp);
}