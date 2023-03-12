#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
using namespace std;

#include <cstdlib>
#include <cstdio>
#include <Windows.h>

typedef unsigned __int16 WORD;//2 байта
//typedef unsigned int DWORD;//4 байта
typedef long LONG;//4 байта
typedef unsigned char BYTE;//1 байт

struct pad {
    size_t before = 0;
    size_t after = 0;
};

int main() {
    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

    FILE* f1, * f2;

    pad padding;

    //remove("pic2.bmp");
    f1 = fopen("pic1 be.bmp", "rb");
    f2 = fopen("pic2.bmp", "wb");

    fread(&bfh, sizeof(bfh), 1, f1);
    fread(&bih, sizeof(bih), 1, f1);

    cout << "было: " << bih.biHeight << "*" << bih.biWidth << endl;
    

    LONG WIGTH = bih.biWidth;
    bih.biWidth /= 2;
    LONG HEIGHT = bih.biHeight;
    bih.biHeight /= 2;
    DWORD SIZE = bih.biSizeImage;
    bih.biSizeImage /= 4;

    cout << "стало: " << bih.biHeight << "*" << bih.biWidth << endl;

    padding.before = (4 - (WIGTH * 3) % 4)%4;
    padding.after = (4 - (bih.biWidth * 3) % 4)%4;
    bfh.bfSize = sizeof(bfh) + sizeof(bih) + bih.biSizeImage * sizeof(rgb) + padding.after * bih.biHeight * sizeof(rgb);

    fwrite(&bfh, sizeof(bfh), 1, f2);
    fwrite(&bih, sizeof(bih), 1, f2);

    fseek(f1, bfh.bfOffBits, SEEK_SET);

    cout << "padding.before: " << padding.before << endl;
    cout << "padding.after: " << padding.after << endl;

    int k;
    for (int i = 0; i < HEIGHT; i++) {
        k = 0;
        for (int j = 0; j < WIGTH; j++) {
            fread(&rgb, sizeof(rgb), 1, f1);
            if ((j % 2 == 0) && (i % 2 == 0)  && (k < bih.biWidth )) {
                fwrite(&rgb, sizeof(rgb), 1, f2);
                k++;
            }
        }

        fseek(f1, padding.before, SEEK_CUR);//выравнивание 1

        BYTE cos=255;
        if (i % 2 == 0) {
            for (int i = 0; i < padding.after; i++) {
                fwrite(&cos, 1, 1, f2);
            }
        }
    }

    fclose(f1);
    fclose(f2);

    return 0;
}