/*
* @brief bmpͼ���������
* @author ����� 3140100333
* @date 2017-04-05
*/

#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <windows.h>

// ��ȡbmp����
unsigned char *LoadBitmapFile(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader);

// ԭͼ�ı���һЩ���ص��RGB���ֱ�����
void output(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);

// ͼ��ߴ�任�������x��y��ʾ�ߴ�
void output(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, int y);

#endif