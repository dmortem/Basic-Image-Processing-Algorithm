/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
* @date 2017-04-19
*/

#ifndef SHEAR_H
#define SHEAR_H

#include <windows.h>

// xΪ1��ʾ��x�������У�xΪ2��ʾ��y�������У�dΪ����
unsigned char* shear(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, double d, int *h, int *w);

#endif