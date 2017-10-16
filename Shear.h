/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#ifndef SHEAR_H
#define SHEAR_H

#include <windows.h>

// x为1表示对x轴做错切；x为2表示对y轴做错切；d为参数
unsigned char* shear(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, double d, int *h, int *w);

#endif