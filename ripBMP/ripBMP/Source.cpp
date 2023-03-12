/*режим работы 1 1*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
using namespace std;

typedef unsigned __int16 WORD;//2 байта
typedef unsigned int DWORD;//4 байта
typedef long LONG;//4 байта
typedef unsigned char BYTE;//1 байт

typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, * LPBITMAPFILEHEADER, * PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, * LPBITMAPINFOHEADER, * PBITMAPINFOHEADER;

typedef struct tagRGBTRIPLE {
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} RGBTRIPLE, * PRGBTRIPLE, * NPRGBTRIPLE, * LPRGBTRIPLE;

typedef struct count {
    int Width;
    int Height;
}COU;

int main() {
    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

    int A, B;

    A = B = 1;
    cout << A << ' ' << B;

    FILE* f1, * f2;

    remove("pic2.bmp");
    f1 = fopen("pic1.bmp", "r+b");
    f2 = fopen("pic2.bmp", "w+b");

    int padding;
    fread(&bfh, sizeof(bfh), 1, f1);
    fread(&bih, sizeof(bih), 1, f1);

    bih.biWidth /=2;
    if (((bih.biWidth * 3) % 4) > 0) {//вырвниваем длину строк
        padding = 4 - (bih.biWidth * 3) % 4;
        bih.biWidth += 4 - (bih.biWidth * 3) % 4;
    }

    //bih.biHeight /= 2;
    //if (((bih.biHeight * 3) % 4) > 0) {//вырвниваем длину строк
    //   padding = 4 - (bih.biHeight * 3) % 4;
    //   bih.biHeight += 4 - (bih.biHeight * 3) % 4;
    //}

    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bih.biHeight * bih.biWidth * sizeof(RGBTRIPLE);

    fwrite(&bfh, sizeof(bfh), 1, f2);
    fwrite(&bih, sizeof(bih), 1, f2);

    for (int i = 0; i <= bih.biHeight; i++) {
        for (int j = 0; j <= bih.biWidth; j++) {
            fread(&rgb, sizeof(rgb), 1, f1);
            if ((j % 2 == 1) || (i % 2 == 1)) {
                fwrite(&rgb, sizeof(rgb), 1, f2);
            }
            /*else {
                rgb.rgbtBlue = 0;
                rgb.rgbtGreen = 0;
                rgb.rgbtRed = 0;
                fwrite(&rgb, sizeof(rgb), 1, f2);
            }*/
        }
    }

    /*for (int i = 0; i <= bih.biHeight * bih.biWidth; i++) {
        fread(&rgb, sizeof(rgb), 1, f1);
        if (i % 2 == 0) {
            fwrite(&rgb, sizeof(rgb), 1, f2);
        }
    }*/

    fclose(f1);
    fclose(f2);

    return 0;
}