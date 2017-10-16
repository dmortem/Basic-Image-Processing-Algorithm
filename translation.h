/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <windows.h>

// 垂直方向平移x，水平方向平移y
unsigned char* translation(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, int y);

#endif