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

    BYTE* line1 = NULL;
    BYTE* line2 = NULL;

    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

    FILE* f1 = NULL, * f2 = NULL;

    pad padding;

    int Add = 1;

    try {
        f1 = fopen("pic3.bmp", "rb");
        if (!f1) {
            cout << "файл pic3.bmp не существует\n";
            return 0;
        }
        f2 = fopen("pic2.bmp", "wb");
        if (!f2) {
            cout << "не удалось создать файл pic2.bmp\n";
            fclose(f1);
            return 0;
        }

        cout << "пожалуйста, введите количество пропускаемых пикселей\n";
        cin >> Add;

        fread(&bfh, sizeof(bfh), 1, f1);
        fread(&bih, sizeof(bih), 1, f1);

        if ((Add + 1 > bih.biWidth) || (Add + 1 > bih.biHeight))
        {
            cout << "количество пропускаемых символов слишком большое";
            if (line1)
            {
                delete[] line1;
                line1 = NULL;
            }
            if (line2)
            {
                delete[] line2;
                line2 = NULL;
            }
            if (f1)
            {
                fclose(f1);
                f1 = NULL;
            }
            if (f2)
            {
                fclose(f2);
                f2 = NULL;
            }
            return 0;
        }

        line1 = new BYTE[bih.biWidth * 3];

        cout << "было: " << bih.biHeight << "*" << bih.biWidth << endl;

        LONG WIGTH = bih.biWidth;
        bih.biWidth /= 1 + Add;
        LONG HEIGHT = bih.biHeight;
        bih.biHeight /= 1 + Add;
        DWORD SIZE = bih.biSizeImage;
        bih.biSizeImage /= (1 + Add) * (1 + Add);

        cout << "стало: " << bih.biHeight << "*" << bih.biWidth << endl;

        padding.before = (4 - (WIGTH * 3) % 4) % 4;
        padding.after = (4 - (bih.biWidth * 3) % 4) % 4;
        bfh.bfSize = sizeof(bfh) + sizeof(bih) + bih.biSizeImage * sizeof(rgb) + padding.after * bih.biHeight * sizeof(rgb);

        fwrite(&bfh, sizeof(bfh), 1, f2);
        fwrite(&bih, sizeof(bih), 1, f2);

        line2 = new BYTE[bih.biWidth * 3 + padding.after];
        memset(line2 + bih.biWidth * 3, 0, padding.after * sizeof(BYTE));

        fseek(f1, bfh.bfOffBits, SEEK_SET);

        cout << "padding.before: " << padding.before << endl;
        cout << "padding.after: " << padding.after << endl;

        BYTE cos = 255;

        int k;
        for (int i = 0; i < HEIGHT; i += (Add + 1)) {
            k = 0;
            fread(line1, WIGTH * 3, 1, f1);
            for (int j = 0; j < bih.biWidth; j++) {
                line2[3 * j] = line1[3 * (Add + 1) * j];
                line2[3 * j + 1] = line1[3 * (Add + 1) * j + 1];
                line2[3 * j + 2] = line1[3 * (Add + 1) * j + 2];
            }
            fwrite(line2, bih.biWidth * 3 + padding.after, 1, f2);

            fseek(f1, padding.before + (WIGTH * 3 + padding.before) * Add, SEEK_CUR);//пропуск строки
        }

        fclose(f1);
        fclose(f2);

        delete[] line1;
        delete[] line2;
        line1 = NULL;
        line2 = NULL;

        return 0;
    }
    catch (const std::bad_alloc& ex) {//для new
        if (line1)
        {
            delete[] line1;
            line1 = NULL;
        }
        if (line2)
        {
            delete[] line2;
            line2 = NULL;
        }
        if (f1)
        {
            fclose(f1);
            f1 = NULL;
        }
        if (f2)
        {
            fclose(f2);
            f2 = NULL;
        }

        cout << ex.what();
        return 0;
    }
}