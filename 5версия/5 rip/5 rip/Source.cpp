#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
using namespace std;

typedef unsigned __int16 WORD;//2 байта
//typedef unsigned int DWORD;//4 байта
typedef long LONG;//4 байта
typedef unsigned char BYTE;//1 байт

struct pad {
    size_t before = 0;
    size_t after = 0;
};

class FileWithDes {
    FILE* f = NULL;
public:
    FileWithDes(const char A[],const char B[]) {
        f = fopen(A, B);
        if (!f) {
            cout << "файл "<<A<<" не существует или не удалось создать\n";
            throw;
        }
    };

    FILE* getF() {
        return f;
    };

    ~FileWithDes() {
        fclose(f);
        f = NULL;
    }
};

class Line {
    BYTE* line = NULL;
public:
    Line(size_t n) {
        line = new BYTE[n];
    };

    BYTE* data() {
        return line;
    };

    ~Line() {
        delete[] line;
        line = NULL;
    };
};

int main() {

    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;   

    pad padding;

    int Add = 1;

    try {
        FileWithDes f1("pic1.bmp", "rb");
        FileWithDes f2("pic2.bmp", "wb");

        cout << "пожалуйста, введите количество пропускаемых пикселей\n";
        cin >> Add;

        fread(&bfh, sizeof(bfh), 1, f1.getF());
        fread(&bih, sizeof(bih), 1, f1.getF());

        if ((Add + 1 > bih.biWidth) || (Add + 1 > bih.biHeight))
        {
            cout << "количество пропускаемых символов слишком большое";
            return 0;
        }

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

        fwrite(&bfh, sizeof(bfh), 1, f2.getF());
        fwrite(&bih, sizeof(bih), 1, f2.getF());

        Line line1(WIGTH * 3);
        Line line2(bih.biWidth * 3 + padding.after);
        memset(&line2 + bih.biWidth * 3, 0, padding.after * sizeof(BYTE));

        fseek(f1.getF(), bfh.bfOffBits, SEEK_SET);//что за исключение тут?//при повторных запусках не появляется

        cout << "padding.before: " << padding.before << endl;
        cout << "padding.after: " << padding.after << endl;

        for (int i = 0; i < HEIGHT; i += (Add + 1)) {
            fread(line1.data(), WIGTH * 3, 1, f1.getF());
            for (int j = 0; j < bih.biWidth; j++) {
                line2.data()[3 * j] = line1.data()[3 * (Add + 1) * j];
                line2.data()[3 * j + 1] = line1.data()[3 * (Add + 1) * j + 1];
                line2.data()[3 * j + 2] = line1.data()[3 * (Add + 1) * j + 2];
            }
            fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());

            fseek(f1.getF(), padding.before + (WIGTH * 3 + padding.before) * Add, SEEK_CUR);//пропуск строки
        }
    }
        catch (...) {}

        return 0;
}