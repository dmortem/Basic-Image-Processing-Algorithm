/*
* @brief ͼ���ȡ���ҶȻ����������Ϊ�Ҷ�ͼ
* @author ����� 3140100333
* @date 2017-03-27
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <windows.h> // �ṩBITMAPINFOHEADER

using namespace std;

#define BITMAP_ID 0x4D42	// "MB"����ʾ����bmp�ļ�

// ��ȡbmp����
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

// ����Ҷ�ͼ��**���Կ��ǰ�YUV��RGB�Ļ�����дһ������
void output(char *filename, BITMAPINFOHEADER bitmapInfoHeader, unsigned char *bitmapData, double mul);

BITMAPFILEHEADER bitmapFileHeader;	// ����ͼ��bitmap�ļ�ͷ��**֮������ٿ�����ôд�ú�һ��

int main(int argc, char *argv[])
{
	BITMAPINFOHEADER bitmapInfoHeader;	// bitmap��Ϣͷ
	unsigned char *bitmapData;			// λͼ����
	unsigned char *new_bmpData;			// ��λͼ����
	char filename[100];
	double mul;

	printf("Please input the name with .bmp of input image: ");
	scanf("%s", filename);
	printf("Please input the multiple of Y: ");
	scanf("%lf", &mul);

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);	// ��ȡͼƬ����

	output("gray.bmp", bitmapInfoHeader, bitmapData, mul);

	return 0;
}

// ����: ͨ��ָ�룬����filename ָ����bitmap�ļ������ݡ�
// ͬʱҲ����bitmap��Ϣͷ.����֧��-bitλͼ��
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
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
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID)	// �����ļ�ͷ�ж϶����ļ��Ƿ�Ϊbmp�ļ�
	{
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);	// fread�����filePtr�ļ�ָ���Զ��ƶ�����˸ղŶ����ļ�ͷ������Ĭ��filePtrָ��ľ�����Ϣͷ
																	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);			// SEEK_SET��ʾ�ļ���ͷ��SEEK_CUR��ʾ��ǰλ�ã�SEEK_END��ʾ�ļ���β�������ļ���β����ڶ���������ʾ������ǰ��OffSetλ�ã�������fseek�����Ǵ��ļ���ͷ0����ʼ��ƫ��bfOffBits���λ�ø�ֵ��filePtrָ��
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

void output(char *filename, BITMAPINFOHEADER bitmapInfoHeader, unsigned char *bitmapData, double mul)
{
	unsigned char *gray_bmpData;		// �Ҷ�ͼ����
	double *temp_Y, *temp_U, *temp_V;
	double max = 0;

// �ѻҶ�ͼд���ļ�
	FILE *fp = fopen("gray.bmp", "wb");

// �����ļ�ͷ
	BITMAPFILEHEADER bfg;
	bfg.bfType = BITMAP_ID;
	bfg.bfReserved1 = 0;
	bfg.bfReserved2 = 0;
	bfg.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);	// �Ҷ�ͼ�е�ɫ��.OffBitsָ���Ǵ��ļ�ͷ�����ݿ�ʼ�ĵط�
																									// bfg.bfSize��ֵ��OffBits+���ݿ��С���

// ��������ͷ
	BITMAPINFOHEADER big;
	memcpy(&big, &bitmapInfoHeader, sizeof(BITMAPINFOHEADER));		// ����bitmaoInforHeader�е����ݵ�big	
	// ���ֻ��biSizeImage��biBitCount��Ҫ�޸ġ�biWidth��biHeightָ�Ķ���һ��/һ���ϵ������������Ҷ�ͼ�Ͳ�ɫͼ��������һ�������ϵ�ͨ��������������һ�µ�
	int RowLength = ((bitmapInfoHeader.biWidth + 3) / 4) * 4;

	big.biSizeImage = RowLength * bitmapInfoHeader.biHeight;
	bfg.bfSize = bfg.bfOffBits + big.biSizeImage;
	big.biBitCount = 8;		// 1������8bit���ܸ���0-255����ɫͼ��Ҫ3*8=24

							// ��Ϣͷ������ͷ�����
	fwrite(&bfg, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&big, sizeof(BITMAPINFOHEADER), 1, fp);

// �����������ɫ��
	RGBQUAD *ipRGB2 = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
		ipRGB2[i].rgbRed = ipRGB2[i].rgbGreen = ipRGB2[i].rgbBlue = i;

	fwrite(ipRGB2, sizeof(RGBQUAD), 256, fp);

// �������������
	int m = bitmapInfoHeader.biHeight, n = bitmapInfoHeader.biWidth;
	gray_bmpData = new unsigned char[big.biSizeImage];
	temp_Y = new double[big.biSizeImage];
	temp_U = new double[big.biSizeImage];
	temp_V = new double[big.biSizeImage];

	// RGB -> YUV������bitmapData���˳����BGR: bitmapData[3 * (i*RowLength + j)]��B��bitmapData[3 * (i*RowLength + j)+1]��G��bitmapData[3 * (i*RowLength + j)+2]��R
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
		{
			temp_Y[i*RowLength + j] = 0.11 * bitmapData[3 * (i*RowLength + j)] + 0.59 * bitmapData[3 * (i*RowLength + j) + 1] + 0.3 * bitmapData[3 * (i*RowLength + j) + 2];
			temp_U[i*RowLength + j] = 0.493 * (bitmapData[3 * (i*RowLength + j)] - temp_Y[i*RowLength + j]);
			temp_V[i*RowLength + j] = 0.877 * (bitmapData[3 * (i*RowLength + j) + 2] - temp_Y[i*RowLength + j]);
			if (temp_Y[i*RowLength + j] > max)
				max = temp_Y[i*RowLength + j];
		}

	// ��һ��
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
			gray_bmpData[i*RowLength + j] = (unsigned char)(temp_Y[i*RowLength + j] / max * 255);

	fwrite(gray_bmpData, big.biSizeImage, 1, fp);
	fclose(fp);

// ת��Ϊ��ɫͼ���
	// �ı�Y
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
			temp_Y[i*RowLength + j] *= mul;
	// YUV -> GRB
	unsigned char *new_bmpData = new unsigned char[bitmapInfoHeader.biSizeImage];			// ������λͼ����
	double temp_R, temp_G, temp_B;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < RowLength; j++)
		{
			temp_B = temp_U[i*RowLength + j] / 0.493 + temp_Y[i*RowLength + j];
			temp_R = temp_V[i*RowLength + j] / 0.877 + temp_Y[i*RowLength + j];
			temp_G = (temp_Y[i*RowLength + j] - 0.3 * temp_R - 0.11 * temp_B)/0.59;
			// ����Y����ˣ�ת����ʱҪ�������255
			if (temp_B > 255)
				new_bmpData[3 * (i*RowLength + j)] = 255;
			else new_bmpData[3 * (i*RowLength + j)] = (unsigned char)temp_B;
			if (temp_G > 255)
				new_bmpData[3 * (i*RowLength + j) + 1] = 255;
			else new_bmpData[3 * (i*RowLength + j) + 1] = (unsigned char)temp_G;
			if (temp_R > 255)
				new_bmpData[3 * (i*RowLength + j) + 2] = 255;
			else new_bmpData[3 * (i*RowLength + j) + 2] = (unsigned char)temp_R;
		}

// �����ͼ��
	FILE *fp1 = fopen("result.bmp", "wb");
	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp1);
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp1);
	fwrite(new_bmpData, bitmapInfoHeader.biSizeImage, 1, fp1);
	fclose(fp1);

	return;
}