/*
* @brief ���մ�˹ͼ����ǿ
* @author ����� 3140100333
* @date 2017-05-03
*/

#ifndef LAPLACIANENHANCE_H
#define LAPLACIANENHANCE_H

#include <windows.h>

/* ������˹ͼ����ǿ, ѡ�õ�����ģ��Ϊ
   -1 -1 -1
   -1  8 -1
   -1 -1 -1
*/
unsigned char* Laplacian_Enhance(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);

#endif