/*
* @brief 均值滤波
* @author 吴驰域 3140100333
* @date 2017-05-03
*/

#ifndef MEANFILTER_H
#define MEANFILTER_H

#include <windows.h>

// 均值滤波, (2*w+1)*(2*h+1)的滤波窗口, w和h可以是任意数字，不必一定是奇数
unsigned char* meanfilter(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int w, int h);

#endif
