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

int main() {
	setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

    FILE* f1, * f2;

    remove("pic2.bmp");
    f1 = fopen("pic1.bmp", "r+b");
    f2 = fopen("pic2.bmp", "w+b");

    fread(&bfh, sizeof(bfh), 1, f1);
    fread(&bih, sizeof(bih), 1, f1);

    cout << "было: " << bih.biHeight << "*" << bih.biWidth << endl;
    LONG WIGHT = bih.biWidth;
    size_t padding = 1;
    bih.biWidth /= 2;
    //if (bih.biWidth * 3 % 4 != 0) {//вообще не работает
    //    padding = 4 - (bih.biWidth * 3) % 4;
    //    //padding = 1;
    //}
    
    //bool a1, a2;// xor(((%4)%2))
    //a1 = (WIGHT % 4) % 2;
    //a2 = (bih.biWidth % 4) % 2;
    //if (a1 == a2) {
    //    padding = 0;
    //}
    //else {
    //    padding = 1;
    //}
    //padding = 2;

    //int a1, a2; // разница меду %4
    //a1 = WIGHT % 4;
    //a2 = bih.biWidth % 4;
    //padding = a2 - a1;
    //int mod =0;
    //if (padding < 0) {
    //    mod = padding*2;
    //    padding = 0;
    //}

    //bih.biWidth += padding;
    LONG HEIGHT = bih.biHeight;
    bih.biHeight /= 2;
    DWORD SIZE = bih.biSizeImage;
    bih.biSizeImage /= 4;
    //bih.biSizeImage += bih.biHeight * padding;
    bfh.bfSize = sizeof(bfh) + sizeof(bih) + bih.biSizeImage* sizeof(rgb);
    cout << "стало: " << bih.biHeight << "*" << bih.biWidth << endl;

    fwrite(&bfh, sizeof(bfh), 1, f2);
    fwrite(&bih, sizeof(bih), 1, f2);

    int k;

    cout << "padding: " << padding;

    for (int i = 0; i <= HEIGHT; i++) {
        k = 0;
        for (int j = 0; j <= WIGHT; j++) {
            fread(&rgb, sizeof(rgb), 1, f1);
            if ((j % 2 == 0)  && (i % 2 == 0)) {
                fwrite(&rgb, sizeof(rgb), 1, f2);
                k++;
            }
        }
        if (padding != 0) {
            fread(&rgb, padding, 1, f1);
            fwrite(&rgb, padding, 1, f2);
        }
    }

    fclose(f1);
    fclose(f2);

	return 0;
}