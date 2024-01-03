#include "bmpinp.h"
struct Filter {
    Filter(){};
    ~Filter(){};
    void GrayscaleFilter(Image& image) const;
};

struct Negative : Filter {
    explicit Negative(Image& image);
    void NegativeFilter(Image& image) const;
};

struct Grayscale : Filter {
    explicit Grayscale(Image& image);
};

struct Crop : Filter {
    explicit Crop(Image& image, int width, int height);
    void CropFilter(Image& image, int width, int height) const;
};

struct Matrix : Filter {
    explicit Matrix(){};
    bool MatrixFilter(Image& image, std::vector<float> matrix) const;
};

struct Edge : Matrix {
    explicit Edge(Image& image, float threshold);
    void EdgeFilter(Image& image, float threshold) const;
};

struct Sharp : Matrix {
    explicit Sharp(Image& image);
};

struct Blur : Filter {
    explicit Blur(Image& image, float sigma);
    void BlurFilter(Image& image, float sigma);
};

struct Crystal : Filter {
    explicit Crystal(Image& image, int amount);
    void CrysFilter(Image& image, int size);
};