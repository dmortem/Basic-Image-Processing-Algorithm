/*
* @brief bmpͼ������
* @author ����� 3140100333
* @date 2017-04-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "image_io.h"

using namespace std;

#define BITMAP_ID 0x4D42

// ����: ͨ��ָ�룬����filename ָ����bitmap�ļ������ݡ�
// ͬʱҲ����bitmap��Ϣͷ.����֧��-bitλͼ��
unsigned char *LoadBitmapFile(char *filename, BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;						// �ļ�ָ��
	unsigned char *bitmapImage;			// bitmapͼ������
	unsigned int imageIdx = 0;			// ͼ��λ������
	unsigned char tempRGB;				// ��������

	// �ԡ�������+����ģʽ���ļ�filename�����ļ��󼴿�ʼ�������ļ����ļ�ͷ����Ϣͷ����ɫ�塢���� 
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
	{
		printf("Open file error!\n");
		printf("Please input like this: Monet.bmp");
		return NULL;
	}

	// ����bitmap�ļ�ͼ
	fread(bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader->bfType != BITMAP_ID)	// �����ļ�ͷ�ж϶����ļ��Ƿ�Ϊbmp�ļ�
	{
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);	// fread�����filePtr�ļ�ָ���Զ��ƶ�����˸ղŶ����ļ�ͷ������Ĭ��filePtrָ��ľ�����Ϣͷ
																	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);			// SEEK_SET��ʾ�ļ���ͷ��SEEK_CUR��ʾ��ǰλ�ã�SEEK_END��ʾ�ļ���β�������ļ���β����ڶ���������ʾ������ǰ��OffSetλ�ã�������fseek�����Ǵ��ļ���ͷ0����ʼ��ƫ��bfOffBits���λ�ø�ֵ��filePtrָ��
																	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL)
	{
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}