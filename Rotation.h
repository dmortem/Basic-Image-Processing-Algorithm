/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
* @date 2017-04-19
*/

#ifndef ROTATION_H
#define ROTATION_H

#include <windows.h>

// ��ԭ����תa��
unsigned char* rotation(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, double a, int *h ,int *w);

#endif
