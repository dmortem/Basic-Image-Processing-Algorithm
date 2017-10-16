/*
* @brief ˫���˲�
* @author ����� 3140100333
* @date 2017-05-10
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "image_io.h"
#include "Bilateral_Filter.h"

using namespace std;

#define BITMAP_ID 0x4D42

int main(int argv, char* argc[])
{
	BITMAPINFOHEADER bitmapInfoHeader;		// bitmap��Ϣͷ
	BITMAPFILEHEADER bitmapFileHeader;		// bitmap�ļ�ͷ
	unsigned char *bitmapData;				// λͼ����
	unsigned char *bifilterbitmapData;		// ˫���˲�������
	double sigma_space, sigma_color;
	char filename[100];
	int d;

	printf("Please input the name with .bmp of input image: ");
	scanf("%s", filename);

	printf("Please input the size of the kernel in bilater filter: ");
	scanf("%d", &d);

	printf("Please input sigma_color and sigma_space: ");
	scanf("%lf%lf", &sigma_color, &sigma_space);

	bitmapData = LoadBitmapFile(filename, &bitmapFileHeader, &bitmapInfoHeader);	// ��ȡͼƬ���ݣ�������Ϣͷ

																					// ��ֵ�˲�
	bifilterbitmapData = Bilateral_Filter(&bitmapFileHeader, &bitmapInfoHeader, bitmapData, d, sigma_color, sigma_space);
	output("bilateral_filter.bmp", &bitmapFileHeader, &bitmapInfoHeader, bifilterbitmapData);

	system("pause");
	return 0;
}