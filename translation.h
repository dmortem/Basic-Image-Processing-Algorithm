/*
* @brief ͼ�񼸺α任
* @author ����� 3140100333
* @date 2017-04-19
*/

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <windows.h>

// ��ֱ����ƽ��x��ˮƽ����ƽ��y
unsigned char* translation(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, int y);

#endif