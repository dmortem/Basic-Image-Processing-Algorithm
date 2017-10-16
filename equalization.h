/*
* @brief 直方图均衡化
* @author 吴驰域 3140100333
* @date 2017-04-12
*/

#ifndef EQUALIZATION_H
#define EQUALIZATION_H

#include <windows.h>

// 对RGB三个通道分别做直方图均衡化
unsigned char* equalization(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);	

unsigned char* equalization_grey(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);

#endif