/*
* @brief ˫���˲�
* @author ����� 3140100333
* @date 2017-05-10
*/

#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

#include <windows.h>

/* ˫���˲���dΪ����ֱ����sigma_colorΪ��ɫ�ϵĸ�˹����Ȩֵ��sigma_spaceΪ�ռ��ϵĸ�˹����Ȩֵ */
unsigned char* Bilateral_Filter(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int d, double sigma_color, double sigma_space);

#endif