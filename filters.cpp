#include "filters.h"

void Negative::NegativeFilter(Image &image) const {
    const int full = 1.f;
    for (int i = 0; i < image.InfoHeader.biHeight; ++i) {
        for (int j = 0; j < image.InfoHeader.biWidth; ++j) {
            image.img[i][j].b = full - image.img[i][j].b;
            image.img[i][j].g = full - image.img[i][j].g;
            image.img[i][j].r = full - image.img[i][j].r;
        }
    }
}

void Filter::GrayscaleFilter(Image &image) const {
    const float red_gray = 0.299;
    const float green_gray = 0.587;
    const float blue_gray = 0.114;

    for (int i = 0; i < image.InfoHeader.biHeight; ++i) {
        for (int j = 0; j < image.InfoHeader.biWidth; ++j) {
            float color =
                (image.img[i][j].r * red_gray) + (image.img[i][j].g * green_gray) + (image.img[i][j].b * blue_gray);
            image.img[i][j].b = color;
            image.img[i][j].g = color;
            image.img[i][j].r = color;
        }
    }
}
Crop::Crop(Image &image, int width, int height) {
    CropFilter(image, width, height);
}

void Crop::CropFilter(Image &image, int width, int height) const {
    std::vector<std::vector<BGR>> new_img(height, std::vector<BGR>(width));
    if (height >= image.InfoHeader.biHeight) {
        height = image.InfoHeader.biHeight;
    }
    if (width >= image.InfoHeader.biWidth) {
        width = image.InfoHeader.biWidth;
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            new_img[height - 1 - i][j] = image.img[image.InfoHeader.biHeight - i - 1][j];
        }
    }
    image.img = new_img;
    image.InfoHeader.biWidth = width;
    image.InfoHeader.biHeight = height;
    image.InfoHeader.biSizeImage = image.RealWidth() * image.InfoHeader.biHeight;
    image.FileHeader.bfSize = image.InfoHeader.biSizeImage + image.FileHeader.bfOffBits;
}

bool Matrix::MatrixFilter(Image &image, std::vector<float> matrix) const {
    const float full = 1.f;
    const int nine = 9;
    if (matrix.size() != nine) {
        return false;
    }
    const float a = matrix[0];
    const float b = matrix[1];
    const float c = matrix[2];
    const float d = matrix[3];
    const float e = matrix[4];
    const float f = matrix[5];
    const float g = matrix[6];
    const float h = matrix[7];
    const float i = matrix[8];

    std::vector<std::vector<BGR>> new_img =
        std::vector<std::vector<BGR>>(image.InfoHeader.biHeight, std::vector<BGR>(image.InfoHeader.biWidth));

    for (int k = 0; k < image.InfoHeader.biHeight; ++k) {
        int i1 = k - 1;
        int i2 = k;
        int i3 = k + 1;
        if (k == 0) {
            i1 = k;
        } else if (k == (image.InfoHeader.biHeight - 1)) {
            i3 = k;
        }
        for (int j = 0; j < image.InfoHeader.biWidth; ++j) {
            int j1 = j - 1;
            int j2 = j;
            int j3 = j + 1;
            if (j == 0) {
                j1 = j;
            } else if (j == (image.InfoHeader.biWidth - 1)) {
                j3 = j;
            }
            new_img[k][j].b = std::min(
                full, std::max(0.f, a * image.img[i1][j1].b + b * image.img[i1][j2].b + c * image.img[i1][j3].b +
                                        d * image.img[i2][j1].b + e * image.img[i2][j2].b + f * image.img[i2][j3].b +
                                        g * image.img[i3][j1].b + h * image.img[i3][j2].b + i * image.img[i3][j3].b));
            new_img[k][j].g = std::min(
                full, std::max(0.f, a * image.img[i1][j1].g + b * image.img[i1][j2].g + c * image.img[i1][j3].g +
                                        d * image.img[i2][j1].g + e * image.img[i2][j2].g + f * image.img[i2][j3].g +
                                        g * image.img[i3][j1].g + h * image.img[i3][j2].g + i * image.img[i3][j3].g));
            new_img[k][j].r = std::min(
                full, std::max(0.f, a * image.img[i1][j1].r + b * image.img[i1][j2].r + c * image.img[i1][j3].r +
                                        d * image.img[i2][j1].r + e * image.img[i2][j2].r + f * image.img[i2][j3].r +
                                        g * image.img[i3][j1].r + h * image.img[i3][j2].r + i * image.img[i3][j3].r));
        }
    }
    image.img = new_img;
    return true;
}

void Edge::EdgeFilter(Image &image, float threshold) const {
    const int full = 1.f;
    const float four = 4.f;
    GrayscaleFilter(image);
    MatrixFilter(image, {0.f, -1.f, 0.f, -1.f, four, -1.f, 0.f, -1.f, 0.f});
    for (int i = 0; i < image.InfoHeader.biHeight; ++i) {
        for (int j = 0; j < image.InfoHeader.biWidth; ++j) {
            if (static_cast<float>(image.img[i][j].b) > threshold) {
                image.img[i][j].b = full;
                image.img[i][j].g = full;
                image.img[i][j].r = full;
            } else {
                image.img[i][j].b = 0;
                image.img[i][j].g = 0;
                image.img[i][j].r = 0;
            }
        }
    }
}

Negative::Negative(Image &image) {
    NegativeFilter(image);
}

Grayscale::Grayscale(Image &image) {
    GrayscaleFilter(image);
}

Edge::Edge(Image &image, float threshold) {
    EdgeFilter(image, threshold);
}

Sharp::Sharp(Image &image) {
    const float five = 5.f;
    MatrixFilter(image, {0.f, -1.f, 0.f, -1.f, five, -1.f, 0.f, -1.f, 0.f});
}
Blur::Blur(Image &image, float sigma) {
    BlurFilter(image, sigma);
}

void Blur::BlurFilter(Image &image, float sigma) {
    const double eps = 0.0001;
    int i = 0;
    double znam = std::sqrt(2 * atan(1) * 4 * sigma);
    double znam2 = 2 * (sigma * sigma);
    std::vector<double> gauss;
    double f = exp(static_cast<double>(-(i * i) / znam2)) / znam;
    while (f > eps) {
        gauss.push_back(f);
        ++i;
        f = exp(static_cast<double>(-(i * i) / znam2)) / znam;
    }
    std::reverse(gauss.begin(), gauss.end());
    gauss.resize(i * 2 - 1);
    std::copy_n(gauss.begin(), i - 1, gauss.rbegin());
    double sum = std::accumulate(gauss.begin(), gauss.end(), 0.f);

    for (int j = 0; j < 2 * i - 1; ++j) {
        gauss[j] /= sum;
    }
    std::vector<std::vector<BGRD>> new_img(image.InfoHeader.biHeight, std::vector<BGRD>(image.InfoHeader.biWidth));
    for (int y = 0; y != image.InfoHeader.biHeight; ++y) {
        for (int x = 0; x != image.InfoHeader.biWidth; ++x) {
            new_img[y][x].b = image.img[y][x].b;
            new_img[y][x].r = image.img[y][x].r;
            new_img[y][x].g = image.img[y][x].g;
        }
    }
    int max = 2 * i - 1;

    for (int y = 0; y < image.InfoHeader.biHeight; ++y) {
        for (int x = 0; x < image.InfoHeader.biWidth; ++x) {
            double blue = 0;
            double green = 0;
            double red = 0;
            for (int dx = -(max / 2); dx <= max / 2; ++dx) {
                int cur_x = x + dx;
                if (cur_x >= image.InfoHeader.biWidth - 1) {
                    cur_x = image.InfoHeader.biWidth - 1;
                } else if (cur_x <= 0) {
                    cur_x = 0;
                }
                blue += new_img[y][cur_x].b * gauss[dx + max / 2];
                green += new_img[y][cur_x].g * gauss[dx + max / 2];
                red += new_img[y][cur_x].r * gauss[dx + max / 2];
            }
            new_img[y][x].b = blue;
            new_img[y][x].g = green;
            new_img[y][x].r = red;
        }
    }

    for (int y = 0; y < image.InfoHeader.biHeight; ++y) {
        for (int x = 0; x < image.InfoHeader.biWidth; ++x) {
            double blue = 0;
            double green = 0;
            double red = 0;
            for (int dy = -(max / 2); dy <= max / 2; ++dy) {
                int cur_y = y + dy;

                if (cur_y >= image.InfoHeader.biHeight - 1) {
                    cur_y = image.InfoHeader.biHeight - 1;
                } else if (cur_y <= 0) {
                    cur_y = 0;
                }
                blue += new_img[cur_y][x].b * gauss[dy + max / 2];
                green += new_img[cur_y][x].g * gauss[dy + max / 2];
                red += new_img[cur_y][x].r * gauss[dy + max / 2];
            }
            image.img[y][x].b = static_cast<float>(blue);
            image.img[y][x].g = static_cast<float>(green);
            image.img[y][x].r = static_cast<float>(red);
        }
    }
}

Crystal::Crystal(Image &image, int size) {
    CrysFilter(image, size);
}

void Crystal::CrysFilter(Image &image, int size) {
    std::vector<std::vector<BGR>> new_img(image.InfoHeader.biHeight, std::vector<BGR>(image.InfoHeader.biWidth));
    new_img = image.img;
    std::vector<int> cur_x(size);
    std::vector<int> cur_y(size);

    for (int i = 0; i < size; ++i) {
        cur_x[i] = rand() % image.InfoHeader.biWidth;
        cur_y[i] = rand() % image.InfoHeader.biHeight;
    }

    for (int y = 0; y < image.InfoHeader.biHeight; ++y) {
        for (int x = 0; x < image.InfoHeader.biWidth; ++x) {
            double min_dist = (y - cur_y[0]) * (y - cur_y[0]) + (x - cur_x[0]) * (x - cur_x[0]);
            int j = 0;
            for (int i = 1; i < size; ++i) {
                double current_distance_sq = (y - cur_y[i]) * (y - cur_y[i]) + (x - cur_x[i]) * (x - cur_x[i]);
                if (current_distance_sq < min_dist) {
                    j = i;
                    min_dist = current_distance_sq;
                }
            }
            image.img[y][x] = new_img[cur_y[j]][cur_x[j]];
        }
    }
}
