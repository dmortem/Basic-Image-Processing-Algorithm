/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#ifndef ROTATION_H
#define ROTATION_H

#include <windows.h>

// 绕原点旋转a°
unsigned char* rotation(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, double a, int *h ,int *w);

#endif
