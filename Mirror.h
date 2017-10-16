/*
* @brief 图像几何变换
* @author 吴驰域 3140100333
* @date 2017-04-19
*/

#ifndef MIRROR_H
#define MIRROR_H

#include <windows.h>

// 图像做镜像变换，sx=1&&sy=-1表示绕x轴转，sx=-1&&sy=1表示绕y轴转
unsigned char* mirror(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int sx, int sy);

#endif

