/*
* @brief ֱ��ͼ���⻯
* @author ����� 3140100333
* @date 2017-04-12
*/

#ifndef EQUALIZATION_H
#define EQUALIZATION_H

#include <windows.h>

// ��RGB����ͨ���ֱ���ֱ��ͼ���⻯
unsigned char* equalization(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);	

unsigned char* equalization_grey(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);

#endif