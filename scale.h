/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
* @date 2017-04-19
*/

#ifndef SCALE_H
#define SCALE_H

#include <windows.h>

// ��ֱ��������x��ˮƽ��������y
unsigned char* scale(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, double x, double y, int *h, int *w);

#endif