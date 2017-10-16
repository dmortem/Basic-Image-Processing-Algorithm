/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#ifndef SCALE_H
#define SCALE_H

#include <windows.h>

// 垂直方向缩放x，水平方向缩放y
unsigned char* scale(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, double x, double y, int *h, int *w);

#endif