/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
* @date 2017-04-19
*/

#ifndef MIRROR_H
#define MIRROR_H

#include <windows.h>

// ͼ��������任��sx=1&&sy=-1��ʾ��x��ת��sx=-1&&sy=1��ʾ��y��ת
unsigned char* mirror(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int sx, int sy);

#endif

