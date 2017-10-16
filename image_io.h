/*
* @brief bmp图像输入输出
* @author 吴驰域 3140100333
* @date 2017-04-05
*/

#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <windows.h>

// 读取bmp函数
unsigned char *LoadBitmapFile(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader);

// 原图改变了一些像素点的RGB后的直接输出
void output(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData);

// 图像尺寸变换后输出。x和y表示尺寸
void output(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, unsigned char *bitmapData, int x, int y);

#endif