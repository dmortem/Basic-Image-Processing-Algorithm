/*
* @brief ��ֵ�˲�
* @author ����� 3140100333
* @date 2017-05-03
*/

#ifndef MEANFILTER_H
#define MEANFILTER_H

#include <windows.h>

// ��ֵ�˲�, (2*w+1)*(2*h+1)���˲�����, w��h�������������֣�����һ��������
unsigned char* meanfilter(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int w, int h);

#endif
