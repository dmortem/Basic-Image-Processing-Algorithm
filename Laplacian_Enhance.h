/*
* @brief 拉普达斯图像增强
* @author 吴驰域 3140100333
* @date 2017-05-03
*/

#ifndef LAPLACIANENHANCE_H
#define LAPLACIANENHANCE_H

#include <windows.h>

/* 拉普拉斯图像增强, 选用的算子模板为
   -1 -1 -1
   -1  8 -1
   -1 -1 -1
*/
unsigned char* Laplacian_Enhance(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);

#endif