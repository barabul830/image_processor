#include "bmpinp.h"

// typedef unsigned long DWORD;
// typedef unsigned short WORD;
// typedef long LONG;

int Image::RealWidth() const {
    return InfoHeader.biWidth * 3 + (4 - InfoHeader.biWidth * 3 % 4) % 4;
}

Image::Image(const unsigned char *in_rgb, BITMAPFILEHEADER file_header, BITMAPINFOHEADER info_header) {
    this->FileHeader = file_header;
    this->InfoHeader = info_header;
    const float full = 255;
    int rw = RealWidth();
    img = std::vector<std::vector<BGR>>(InfoHeader.biHeight, std::vector<BGR>(InfoHeader.biWidth));
    for (LONG i = 0; i < InfoHeader.biHeight; ++i) {
        for (LONG j = 0; j < InfoHeader.biWidth; ++j) {
            img[i][j].b = static_cast<float>(in_rgb[i * rw + 3 * j + 0]) / full;
            img[i][j].g = static_cast<float>(in_rgb[i * rw + 3 * j + 1]) / full;
            img[i][j].r = static_cast<float>(in_rgb[i * rw + 3 * j + 2]) / full;
        }
    }
}

unsigned char *Image::OutRGB() {
    const int full = 255;
    Iz_RGB = static_cast<unsigned char *>(malloc(FileHeader.bfSize - FileHeader.bfOffBits));
    for (size_t i = 0; i < FileHeader.bfSize - FileHeader.bfOffBits; ++i) {
        Iz_RGB[i] = 0;
    }
    int rw = RealWidth();
    for (int i = 0; i < InfoHeader.biHeight; ++i) {
        for (int j = 0; j < InfoHeader.biWidth; ++j) {
            Iz_RGB[i * rw + 3 * j + 0] = static_cast<unsigned char>(img[i][j].b * full);
            Iz_RGB[i * rw + 3 * j + 1] = static_cast<unsigned char>(img[i][j].g * full);
            Iz_RGB[i * rw + 3 * j + 2] = static_cast<unsigned char>(img[i][j].r * full);
        }
    }
    return Iz_RGB;
}