/*
* @brief 双边滤波
* @author 吴驰域 3140100333
* @date 2017-05-10
*/

#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

#include <windows.h>

/* 双边滤波，d为邻域直径，sigma_color为颜色上的高斯函数权值，sigma_space为空间上的高斯函数权值 */
unsigned char* Bilateral_Filter(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int d, double sigma_color, double sigma_space);

#endif