#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <numeric>

using WORD = uint16_t;
using DWORD = uint32_t;
using LONG = int32_t;

using BITMAPFILEHEADER = struct __attribute__((packed)) TagBitmapfileheader {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
};

using BITMAPINFOHEADER = struct __attribute__((packed)) TagBitmapinfoheader {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};

struct BGR {
    float b = 0, g = 0, r = 0;

    BGR(){};
    ~BGR(){};

    BGR(float b, float g, float r) : b(b), g(g), r(r){};
};

struct BGRD {
    double b = 0, g = 0, r = 0;

    BGRD(){};
    ~BGRD(){};

    BGRD(double b, double g, double r) : b(b), g(g), r(r){};
};

struct Image {
    BITMAPINFOHEADER InfoHeader;
    BITMAPFILEHEADER FileHeader;
    std::vector<std::vector<BGR>> img;
    unsigned char* Iz_RGB;

    int RealWidth() const;
    Image(){};

    Image(const unsigned char* in_rgb, BITMAPFILEHEADER file_header, BITMAPINFOHEADER info_header);

    unsigned char* OutRGB();

    ~Image() {
        free(Iz_RGB);
    };
};
